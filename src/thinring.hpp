#ifndef THINRING_HPP
#define THINRING_HPP
#include "intersection.hpp"
#include "solid_reorientable.hpp"

namespace raytracing {
namespace Imager {
struct Vector;

// A thin ring is a zero-thickness circular disc with an optional
// disc-shaped hole in the center.
class ThinRing : public SolidObject_Reorientable {
public:
	explicit ThinRing(double inner_radius, double outer_radius);

protected:
	IntersectionList object_space_append_all_intersections(
		const Vector &vantage, const Vector &direction) const override;

	bool object_space_contains(const Vector &point) const override;

private:
	double inner_radius_; // The radius of the hole at the center of the ring.
	double outer_radius_; // The outer radius of the ring.

	/*
	 * \brief A temporary intersection list, cached inside this object to avoid
	 * repeated memory allocations.
	 *
	 * Marked mutable to allow const functions to cache lists whose memory may
	 * be reused as needed.
	 */
	mutable IntersectionList temp_intersection_list_;
};

//------------------------------------------------------------------------

/*
 * \brief A thin disc is a zero-thickness disc.
 *
 * It is implemented as a thin ring with a zero-radius hole.
 */
struct ThinDisc : ThinRing {
	ThinDisc(double radius);
};
} // namespace Imager
} // namespace raytracing
#endif /* ifndef THINRING_HPP */
