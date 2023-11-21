#ifndef INTERSECTION_HPP
#define INTERSECTION_HPP

#include "algebra.hpp"
#include "vector.hpp"

#include <fmt/format.h>
#include <vector>

namespace raytracing {

namespace Imager {
class SolidObject;

// Provides information about a ray intersecting with a point on the surface of
// a SolidObject.

struct Intersection {
	// The square of the distance from the vantage point to the intersection
	// point.
	double distance_squared = 1.0e+20; // larger than any reasonable value

	Vector point;

	// The unit vector perpendicular to the surface at the intersection point.
	Vector surface_normal;

	// A pointer to the solid object that the ray intersected with.
	const SolidObject *solid;

	// An optional tag for classes derived from SolidObject to cache
	// arbitrary information about surface optics. Most classes can
	// safely leave this pointer as NULL, its default value.
	const void *context;

	// An optional tag used for debugging.
	// Anything that finds an intersection may elect to make tag point at a
	// static string to help the programmer figure out, for example, which of
	// multiple surfaces was involved. This is just a char* instead of
	// std::string to minimize overhead by eliminating dynamic memory
	// allocation.
	const char *tag;
	auto operator<=>(const Intersection &) const = default;

	auto operator==(const Intersection &other) const noexcept {
		if (!Algebra::is_equal(distance_squared, other.distance_squared))
			return false;
		if (point != other.point) return false;
		if (surface_normal != other.surface_normal) return false;
		if (solid != other.solid) return false;
		return true;
	}
};

using IntersectionList
	= std::vector<Intersection>;

enum class IntersectionResultType { NOT_FOUND, UNIQUE, AMBIGIOUS, ACCEPTABLE };

/*
 * \brief Imager::IntersectionResultis a struct that provides a way to store
 an Intersection object and a enum flag as a single unit.
 * This struct has no base classes or declared members other than those
 shown below. Thus it is suitable for use with structured bindings.
 */
struct IntersectionResult {
	// a value (that is supposed to be a stored value)
	Intersection value;
	// an enum flag (that may show if an appropriate intersection can be
	// found)
	IntersectionResultType err;
};

IntersectionResult pick_closest_intersection(const IntersectionList &list);
} // namespace Imager

} // namespace raytracing

template <>
struct fmt::formatter<raytracing::Imager::Intersection> {
	constexpr auto parse(fmt::format_parse_context &ctx) { return ctx.begin(); }

	auto format(const raytracing::Imager::Intersection &intersection,
				fmt::format_context &ctx) {
		const auto
			&[distance_squared, point, surface_normal, solid, context, tag]
			= intersection;
		return fmt::format_to(ctx.out(),
							  "(point = {}, "
							  "surface_normal = {}, solid = {})",
							  distance_squared,
							  point,
							  surface_normal);
	}
};
#endif /* ifndef INTERSECTION_HPP */
