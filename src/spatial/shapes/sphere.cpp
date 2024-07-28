#include "sphere.hpp"

#include <boost/math/tools/roots.hpp>
#include <mp-units/systems/si/unit_symbols.h>
#include <cmath>
#include <optional>
#include <utility>
#include <algorithm>

namespace raytracing {
class Material;

Sphere::Sphere(Point3fConstRef center, const Material &material,
			   RadiusInMetre radius)
	: SolidObject(center, material), radius_(radius) {}

/*Ray3D Sphere::object_to_camera_coordinates(
	Point3fConstRef origin, Vector3fConstRef direction) const {
	return {model_matrix_ * (origin + center_),
			model_matrix_.linear() * direction};
}

Ray3D Sphere::object_to_camera_coordinates(const Ray3D &object) const {
	return {model_matrix_ * (object.origin + center_),
			model_matrix_.linear() * object.direction};
}*/

Vector3Df Sphere::displacement_to(Vector3fConstRef point) const {
	return point - center();
}

float Sphere::radius2() const {
	using mp_units::si::unit_symbols::m2;
	return (radius_ * radius_).numerical_value_in(m2);
}

bool Sphere::contains(Point3fConstRef point) const {
	return displacement_to(point).squaredNorm() <= radius2();
}

std::optional<float> Sphere::hit(const Ray3D &ray) const {
	using boost::math::tools::quadratic_roots;

	const Vector3Df displacement = displacement_to(ray.origin());
	const auto roots = quadratic_roots(ray.direction().squaredNorm(),
									   2 * ray.direction().dot(displacement),
									   displacement.squaredNorm() - radius2());

	const auto min_postive = std::min(roots.first, roots.second);
	if (std::isnan(min_postive) || min_postive <= 1e-2) return std::nullopt;

	return std::make_optional<float>(min_postive);
}

Vector3Df Sphere::normal_at(Point3fConstRef point) const {
	// gsl_Expects(
	// FPC::near(displacement_to(point).squaredNorm(), radius_ * radius_));

	return displacement_to(point).normalized();
}

} // namespace raytracing
