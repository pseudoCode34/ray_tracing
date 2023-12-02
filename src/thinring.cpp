#include "thinring.hpp"

#include "algebra.hpp"
#include "vector.hpp"

#include <cmath>
#include <cstdlib>

namespace raytracing {
namespace Imager {
ThinRing::ThinRing(double inner_radius, double outer_radius)
	: SolidObject_Reorientable(),
	  inner_radius_(inner_radius),
	  outer_radius_(outer_radius) {}

bool ThinRing::object_space_contains(const Vector &point) const {
	if (std::abs(point.z) > EPSILON) return false;

	const double MAG_SQUARED = point.magnitude_sqr_in_plane();
	using Algebra::is_near_zero;
	return is_near_zero(inner_radius_ * inner_radius_ - MAG_SQUARED)
		   && is_near_zero(MAG_SQUARED - outer_radius_ * outer_radius_);
}

IntersectionList
ThinRing::object_space_append_all_intersections(const Vector &vantage,
												const Vector &direction) const {
	if (fabs(direction.z) <= EPSILON) return {};
	const double U = -vantage.z / direction.z;
	if (U <= EPSILON) return {};
	const double X = U * direction.x + vantage.x;
	const double Y = U * direction.y + vantage.y;
	const double M = X * X + Y * Y;
	if (M > outer_radius_ * outer_radius_ + EPSILON
		|| inner_radius_ * inner_radius_ > M + EPSILON)
		return {};

	return IntersectionList{Intersection{
		.distance_squared = (U * direction).magnitude_squared(),
		.point            = Vector{X, Y, 0.0},

		// We "cheat" a little bit in calculating the normal vector
		// by knowing too much about the caller. This is necessary
		// because this is not really a normal solid object, but an
		// infinitesimally thin surface. Therefore, we provide a
		// different normal vector depending on the supplied
		// vantage, such that the point of view of the observer
		// determines which side of the surface is seen. (Doing
		// otherwise would cause the surface to appear completely
		// black in some cases.)
		.surface_normal = Vector{0.0, 0.0, (vantage.z >= 0.0) ? +1.0 : -1.0},
		.solid          = this,
	}};
}

ThinDisc::ThinDisc(double radius) : ThinRing(0.0, radius) {}

} // namespace Imager
} // namespace raytracing
