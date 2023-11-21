#include "common_constants.hpp"
#include "intersection.hpp"
#include "polyhedra.hpp"
#include "vector.hpp"

#include <math.h>

namespace raytracing {
namespace Imager {
Cuboid::Cuboid(double half_width, double half_length, double half_height)
	: SolidObject_Reorientable(),
	  HALF_WIDTH(half_width),
	  HALF_LENGTH(half_length),
	  HALF_HEIGHT(half_height) {}

bool Cuboid::object_space_contains(const Vector &point) const {
	return (fabs(point.x) <= HALF_WIDTH + EPSILON)
		   && (fabs(point.y) <= HALF_LENGTH + EPSILON)
		   && (fabs(point.z) <= HALF_HEIGHT + EPSILON);
}

IntersectionList
Cuboid::object_space_append_all_intersections(const Vector &vantage,
											  const Vector &direction) const {
	IntersectionList intersection_list;
	double u = NAN;

	// Check for intersections with left/right faces: x = +a or x = -a.
	if (!Algebra::is_fp_zero(direction.x)) {
		// right face (x = +a)
		u = (HALF_WIDTH - vantage.x) / direction.x;
		if (u > EPSILON) {
			Vector displacement = u * direction;
			Vector location     = vantage + displacement;
			if (object_space_contains(location))
				intersection_list.push_back(Intersection{
					.distance_squared = displacement.magnitude_squared(),
					.point            = location,
					.surface_normal   = Vector{+1.0, 0.0, 0.0},
					.solid            = this,
					.tag              = "right face"});
		}

		// left face (x = -a)
		u = (-HALF_WIDTH - vantage.x) / direction.x;
		if (u > EPSILON) {
			Vector displacement = u * direction;
			Vector location     = vantage + displacement;
			if (object_space_contains(location))
				intersection_list.push_back(Intersection{
					.distance_squared = displacement.magnitude_squared(),
					.point            = location,
					.surface_normal   = Vector{-1.0, 0.0, 0.0},
					.solid            = this,
					.tag              = "left face"});
		}
	}
	// Check for intersections with front/back faces: y = -b or y = +b.
	if (!Algebra::is_fp_zero(direction.y)) {
		// front face (y = +b)
		u = (HALF_LENGTH - vantage.y) / direction.y;
		if (u > EPSILON) {
			Vector displacement = u * direction;
			Vector location     = vantage + displacement;
			if (object_space_contains(location)) {
				intersection_list.push_back(Intersection{
					.distance_squared = displacement.magnitude_squared(),
					.point            = location,
					.surface_normal   = Vector{0.0, +1.0, 0.0},
					.solid            = this,
					.tag              = "front face"});
			}
		}

		// back face (y = -b)
		u = (-HALF_LENGTH - vantage.y) / direction.y;
		if (u > EPSILON) {
			Vector displacement = u * direction;
			Vector location     = vantage + displacement;
			if (object_space_contains(location))
				intersection_list.push_back(Intersection{
					.distance_squared = displacement.magnitude_squared(),
					.point            = location,
					.surface_normal   = Vector{0.0, -1.0, 0.0},
					.solid            = this,
					.tag              = "back face"});
		}
	}

	// Check for intersections with top/bottom faces: z = -c or z = +c.
	if (!Algebra::is_fp_zero(direction.z)) {
		// top face (z = +c)
		u = (HALF_HEIGHT - vantage.z) / direction.z;
		if (u > EPSILON) {
			Vector displacement = u * direction;
			Vector location     = vantage + displacement;
			if (object_space_contains(location))
				intersection_list.push_back(Intersection{
					.distance_squared = displacement.magnitude_squared(),
					.point            = location,
					.surface_normal   = Vector{0.0, 0.0, +1.0},
					.solid            = this,
					.tag              = "top face"});
		}
		// bottom face (z = -c)
		u = (-HALF_HEIGHT - vantage.z) / direction.z;
		if (u > EPSILON) {
			Vector displacement = u * direction;
			Vector location     = vantage + displacement;
			if (object_space_contains(location))
				intersection_list.push_back(Intersection{
					.distance_squared = displacement.magnitude_squared(),
					.point            = location,
					.surface_normal   = Vector{0.0, 0.0, -1.0},
					.solid            = this,
					.tag              = "bottom face"});
		}
	}
	return intersection_list;
}
} // namespace Imager
} // namespace raytracing
