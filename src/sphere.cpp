#include "sphere.hpp"

#include "floating_point_comparisons.hpp"
#include "material.hpp"
#include "point3f.hpp"
#include "solid_object.hpp"

#include <boost/math/tools/roots.hpp>
#include <utility>

namespace raytracing {

Sphere::Sphere(Point3fConstRef center, const Material &material, float radius)
	: SolidObject(center, material), radius_(radius) {}

bool Sphere::contains(Point3fConstRef point) const {
	const float r2 = radius_ * radius_;
	return !FPC::greater(displacement_to(point).squaredNorm(), r2);
}

float Sphere::hit(const Ray &ray) const {
	using boost::math::tools::quadratic_roots;

	const Vector3f displacement = displacement_to(ray.origin);
	const auto roots
		= quadratic_roots(ray.direction.squaredNorm(),
						  2 * ray.direction.dot(displacement),
						  displacement.squaredNorm() - radius_ * radius_);

	Eigen::Vector2f solutions{roots.first, roots.second};

	return (solutions.array() > 1e-2)
		.select(solutions, Eigen::NumTraits<float>::infinity())
		.minCoeff();
}

Vector3f Sphere::normal_at(Point3fConstRef point) const {
	// gsl_Expects(
	// FPC::near(displacement_to(point).squaredNorm(), radius_ * radius_));

	return displacement_to(point).normalized();
}
} // namespace raytracing
