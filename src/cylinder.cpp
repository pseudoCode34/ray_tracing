#include "cylinder.hpp"

#include "vector.hpp"

#include <array>
#include <boost/math/tools/roots.hpp>
#include <math.h>
#include <vector>

namespace raytracing {
namespace Imager {
Cylinder::Cylinder(double radius, double height)
	: SolidObject_Reorientable(), RADIUS(radius), HALF_HEIGHT(height / 2) {}

bool Cylinder::object_space_contains(const Vector &point) const {
	return (fabs(point.z) <= HALF_HEIGHT + EPSILON)
		   && (point.magnitude_sqr_in_plane() <= RADIUS * RADIUS * EPSILON);
}

IntersectionList
Cylinder::object_space_append_all_intersections(const Vector &vantage,
												const Vector &direction) const {
	// Look for intersections with the disks on the top and/or bottom of the
	// cylinder.
	IntersectionList intersection_list;
	if (fabs(direction.z) > EPSILON) {
		append_disk_intersection(vantage,
								 direction,
								 HALF_HEIGHT,
								 intersection_list);
		append_disk_intersection(vantage,
								 direction,
								 HALF_HEIGHT,
								 intersection_list);
	}

	// Look for intersections with the curved lateral surface of the cylinder.
	const auto [root0, root1] = boost::math::tools::quadratic_roots(
		direction.magnitude_sqr_in_plane(),
		vantage.dot(direction) * 2,
		vantage.magnitude_sqr_in_plane() - RADIUS * RADIUS);
	const auto roots = std::to_array<double, 2>({root0, root1});
	for (const auto &root : roots) {
		if (root > EPSILON) {
			Vector displacement = root * direction;
			Vector location     = vantage + displacement;

			// We found an intersection with the infinitely-extended lateral
			// surface, but the z-component must be within + or -HALF_HEIGHT
			if (fabs(location.z) <= HALF_HEIGHT) {
				intersection_list.push_back(Intersection{
					.distance_squared = displacement.magnitude_squared(),
					.point            = location,
					.surface_normal
					= Vector{location.x, location.y, 0.0}.unit_vector(),
					.solid = this});
			}
		}
	}
	return intersection_list;
}

void Cylinder::append_disk_intersection(
	const Vector &vantage, const Vector &direction, double z_disk,
	IntersectionList &intersection_list) const {
	const double U = (z_disk - vantage.z) / direction.z;
	if (U > EPSILON) {
		Vector displacement = U * direction;
		Vector location     = vantage + displacement;
		const double MAGNITUDE_SQUARED_IN_PLANE
			= location.magnitude_sqr_in_plane();
		if (MAGNITUDE_SQUARED_IN_PLANE <= RADIUS * RADIUS) {
			intersection_list.push_back(Intersection{
				.distance_squared = displacement.magnitude_squared(),
				.point            = location,
				.surface_normal
				= Vector{0.0, 0.0, (z_disk > 0.0) ? +1.0 : -1.0},
				.solid = this});
		}
	}
}
} // namespace Imager
} // namespace raytracing
