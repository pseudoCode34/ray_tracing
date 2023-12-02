#include "sphere.hpp"

#include "vector.hpp"

#include <array>
#include <boost/math/tools/roots.hpp>
#include <cmath>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/transform.hpp>
#include <ranges>
#include <vector>

namespace raytracing {
namespace Imager {

constexpr std::array<double, 2>
to_array(const std::pair<double, double> &pair) {
	return std::to_array({pair.first, pair.second});
}

Sphere::Sphere() : Sphere(Vector{}, 1) {}

Sphere::Sphere(const Vector &center, double radius)
	: SolidObject(center), radius_(radius) {}

Sphere::Sphere(Vector &&center, double radius)
	: SolidObject(std::move(center)), radius_(radius) {}

std::expected<bool, ContainmentError>
Sphere::contains(const Vector &point) const {
	// Add a little bit to the actual radius to be more tolerant of rounding
	// errors that would incorrectly exclude a point that should be inside the
	// sphere.
	const double R = radius_ + EPSILON;

	// A point is inside the sphere if the square of its distance from the
	// center is within the square of the radius.
	return (point - get_center()).magnitude_squared() <= (R * R);
}

SolidObject &Sphere::rotate(double angle_in_degrees, char axis) {
	return *this;
}

IntersectionList
Sphere::append_all_intersections(const Vector &vantage,
								 const Vector &direction) const {
	// Calculate the coefficients of the quadratic equation
	//     au^2 + bu + c = 0.
	// Solving this equation gives us the roots for any intersection points.
	const Vector DISPLACEMENT = vantage - get_center();
	const double A            = direction.magnitude_squared();
	const double B            = 2.0 * direction.dot(DISPLACEMENT);
	const double C = DISPLACEMENT.magnitude_squared() - radius_ * radius_;
	const auto SOLUTIONS
		= to_array(boost::math::tools::quadratic_roots(A, B, C));

	// Generally, a negative solution means the intersecion is in the opposite
	// of the intended direction from the vantage point. Otherwise, if solution
	// is zero, the intersection is at the vantage point. When the solution of
	// the surface equation is positive, the intersection is in front of the
	// vantage point.
	auto is_positve = [](double solution) {
		// Prevent erroneous intersections from being found too close to the
		// vantage point, which troubles at calculating shadows and lighting
		return solution > EPSILON;
	};

	auto get_intersection_in_front
		= [&vantage, &direction, this](double solution) {
			  const Vector VANTAGE_TO_SURFACE = solution * direction,
						   LOCATION           = vantage + VANTAGE_TO_SURFACE;

			  return Intersection{
				  .distance_squared = VANTAGE_TO_SURFACE.magnitude_squared(),
				  .point            = LOCATION,
				  // The normal vector to the surface of a sphere is outward
				  // from the center.
				  .surface_normal = (LOCATION - get_center()).unit_vector(),
				  .solid          = this};
		  };

	return SOLUTIONS | ranges::views::filter(is_positve)
		   | ranges::views::transform(get_intersection_in_front)
		   | ranges::to<IntersectionList>();
}
} // namespace Imager
} // namespace raytracing
