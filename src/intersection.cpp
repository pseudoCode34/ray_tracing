#include "intersection.hpp"

#include "algebra.hpp"

#include <range/v3/algorithm/min_element.hpp>
#include <stddef.h>

namespace raytracing {
namespace Imager {
IntersectionResult pick_closest_intersection(const IntersectionList &list) {
	// We pick the closest intersection, but we return the number of
	// intersections tied for first place in that contest.  This allows the
	// caller to check for ambiguities in cases where that matters.

	const size_t COUNT = list.size();
	switch (COUNT) {
	case 0:
		// No intersection is available. We leave 'intersection' unmodified. The
		// caller must check the return value to know to avoid using
		// 'intersection'.
		return {Intersection{}, IntersectionResultType::NOT_FOUND};

	case 1:
		// There is exactly one intersection in the given direction, so there is
		// no need to think very hard; just use it!
		return {list.front(), IntersectionResultType::UNIQUE};

	default:

		int tie_count                   = 1;
		const auto CLOSEST_INTERSECTION = *ranges::min_element(
			list,
			[&tie_count](double lhs, double rhs) {
				auto diff = lhs - rhs;
                if (diff < 0.0) {
					tie_count = 1;
					return true;
				}
				if (Algebra::is_near_zero(diff)) ++tie_count;
				return false;
			},
			&Intersection::distance_squared);
		if (tie_count > 1)
			return {CLOSEST_INTERSECTION, IntersectionResultType::AMBIGIOUS};
		return {CLOSEST_INTERSECTION, IntersectionResultType::ACCEPTABLE};
	}
}

} // namespace Imager
} // namespace raytracing
