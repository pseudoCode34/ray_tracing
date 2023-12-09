#include "intersection.hpp"
#include "set.hpp"
#include "solid_object.hpp"

#include <range/v3/core.hpp>
#include <range/v3/view/concat.hpp>
#include <range/v3/view/filter.hpp>

namespace raytracing {
namespace Imager {
struct Vector;

SetIntersection::SetIntersection(const Vector &center, SolidObject *left,
								 SolidObject *right)
	: SolidObject_BinaryOperator(center, left, right) {}

IntersectionList SetIntersection::append_overlapping_intersections(
	const Vector &vantage, const Vector &direction, const SolidObject &a_solid,
	const SolidObject &b_solid) const {
	// Find all the intersections of aSolid with the ray emanating
	// from the vantage point.
	temp_intersection_list
		= a_solid.append_all_intersections(vantage, direction);
	auto is_inside_b_solid = [&b_solid](const Intersection &intersection) {
		// FIXME: error handling here?
		return b_solid.contains(intersection.point).value_or(false);
	};
	return temp_intersection_list | ranges::views::filter(is_inside_b_solid)
		   | ranges::to<IntersectionList>();
}

IntersectionList
SetIntersection::append_all_intersections(const Vector &vantage,
										  const Vector &direction) const {
	auto x
		= append_overlapping_intersections(vantage, direction, left(), right());
	auto y
		= append_overlapping_intersections(vantage, direction, right(), left());
	return ranges::views::concat(x, y) | ranges::to<IntersectionList>();
}

bool SetIntersection::has_overlapping_intersection(
	const Vector &vantage, const Vector &direction, const SolidObject &a_solid,
	const SolidObject &b_solid) const {
	// Find all the intersections of aSolid with the ray emanating from vantage.
	/* temp_intersection_list */
	/* 	= a_solid.append_all_intersections(vantage, direction); */

	/* for (const Intersection &intersection : temp_intersection_list) { */
	/* 	// If bSolid contains any of the intersections with aSolid, then */
	/* 	// aSolid and bSolid definitely overlap at that point. */
	/* 	if (b_solid.contains(intersection.point)) return true; */
	/* } */

	/* // Either there was no intersection with aSolid in this direction */
	/* // from the vantage point, or none of them were contained by bSolid. */
	/* return false; */
	return std::ranges::any_of(
		/* temp_intersection_list, */
		a_solid.append_all_intersections(vantage, direction),
		[&b_solid](const Vector &point) {
			// FIXME: error handling here
			return b_solid.contains(point).value_or(false);
		},
		&Intersection::point);
}

std::expected<bool, ContainmentError>
SetIntersection::contains(const Vector &point) const {
	// A point is inside the set intersection if it is inside both of the
	// nested solids.
	return left().contains(point) && right().contains(point);
}

} // namespace Imager
} // namespace raytracing
