#include "torus.hpp"

#include <boost/math/tools/quartic_roots.hpp>
#include <cmath>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/transform.hpp>
#include <vector>

namespace raytracing {
namespace Imager {
Torus::Torus(double r, double s) : SolidObject_Reorientable(), R(r), S(s) {}

std::vector<double> Torus::solve_intersections(const Vector &vantage,
											   const Vector &direction) const {
	// Set up the coefficients of a quartic equation for the intersection of the
	// parametric equation P = vantage + u*direction and the surface of the
	// torus. There is far too much algebra to explain here. See the text of the
	// tutorial for derivation.

	const double T = 4.0 * R * R;
	const double G = T * direction.magnitude_sqr_in_plane();
	const double H = 2.0 * T * vantage.dot_in_plane(direction);
	const double I = T * vantage.magnitude_sqr_in_plane();
	const double J = direction.magnitude_squared();
	const double K = 2.0 * vantage.dot(direction);
	const double L = vantage.magnitude_squared() + R * R - S * S;

	// We need to keep only the real roots.
	// There can be significant roundoff error in quartic solver,
	// so we have to tolerate more slop than usual.
	const double SURFACE_TOLERANCE = 1.0e-4;

	// receives 0..4 real solutions
	auto roots = boost::math::tools::quartic_roots(J * J,
												   2.0 * J * K,
												   2.0 * J * L + K * K - G,
												   2.0 * K * L - H,
												   L * L - I);
	return roots | ranges::views::filter([SURFACE_TOLERANCE](auto number) {
			   return number > SURFACE_TOLERANCE;
		   })
		   | ranges::to<std::vector<double>>();
}

IntersectionList
Torus::object_space_append_all_intersections(const Vector &vantage,
											 const Vector &direction) const {
	const auto SOLUTIONS = solve_intersections(vantage, direction);
	return SOLUTIONS | ranges::views::transform([&](double solution) {
			   const Vector DISP     = solution * direction,
							LOCATION = vantage + DISP;
			   return Intersection{.distance_squared = DISP.magnitude_squared(),
								   .point            = LOCATION,
								   .surface_normal   = surface_normal(LOCATION),
								   .solid            = this};
		   })
		   | ranges::to<IntersectionList>();
}

Vector Torus::surface_normal(const Vector &point) const {
	// Thanks to the fixed orientation of the torus in object space,
	// it always lies on the xy plane, and centered at <0,0,0>.
	// Therefore, if we drop a line in the z-direction from
	// any point P on the surface of the torus to the xy plane,
	// we find a point P' the same direction as a point Q at the center
	// of the torus tube.  Converting P' to a unit vector and multiplying
	// the result by the magnitude of Q (which is the constant R)
	// gives us the coordinates of Q.  Then the surface normal points
	// in the same direction as the difference P-Q.
	// See the tutorial text for more details.

	const double A = 1.0 - (R / sqrt(point.magnitude_sqr_in_plane()));
	return Vector{A * point.x, A * point.y, point.z}.unit_vector();
}

bool Torus::object_space_contains(const Vector &point) const {
	// See http://en.wikipedia.org/wiki/Torus "Geometry" section about
	// solution of torus as f(x,y,z) = 0.
	// We calculate the same function f here applied to the given point.
	// If f(x,y,z) <= 0 (with EPSILON tolerance for roundoff error), we
	// consider the point inside the torus.

	const double T = R - std::sqrt(point.magnitude_sqr_in_plane());
	const double F = T * T + point.z * point.z - S * S;
	return F <= EPSILON;
}
} // namespace Imager
} // namespace raytracing
