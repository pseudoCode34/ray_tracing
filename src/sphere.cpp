#include "sphere.hpp"

#include "algebra.hpp"
#include "vector.hpp"

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/transform.hpp>

namespace raytracing::Imager {

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

SolidObject &Sphere::rotate([[maybe_unused]] double angle_in_degrees,
							[[maybe_unused]] char axis) {
	return *this;
}

// Solve the quadratic equation to find any intersection points.
IntersectionList
Sphere::append_all_intersections(const Vector &vantage,
								 const Vector &direction) const {
	auto get_intersection_in_front = [&](double solution) -> Intersection {
		const Vector VANTAGE_TO_SURFACE = solution * direction,
					 LOCATION           = vantage + VANTAGE_TO_SURFACE;

		return {.distance_squared = VANTAGE_TO_SURFACE.magnitude_squared(),
				.point            = LOCATION,
				// The normal vector to the surface of a sphere is outward
				// from the center.
				.surface_normal = (LOCATION - get_center()).unit_vector(),
				.solid          = this};
	};

	const Vector DISPLACEMENT = vantage - get_center();
	const auto SOLUTIONS      = Algebra::solve_quadratic(
        direction.magnitude_squared(),
        direction.dot(DISPLACEMENT) * 2,
        DISPLACEMENT.magnitude_squared() - radius_ * radius_);

	return SOLUTIONS
		   | ranges::views::filter(
			   /*
				* \brief Prevent the intersection too close to the vantage point
				* to trouble at calculating shadows and lighting
				*
				* Generally, a negative solution means the intersecion is in the
				* opposite of the intended direction from the vantage point.
				* Otherwise, if solution is zero, the intersection is at the
				* vantage point. When the solution of the surface equation is
				* positive, the intersection is in front of the vantage point.
				*/
			   Algebra::is_positve)
		   | ranges::views::transform(get_intersection_in_front)
		   | ranges::to<IntersectionList>();
}
} // namespace raytracing::Imager
