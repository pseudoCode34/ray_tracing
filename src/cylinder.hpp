#ifndef CYLINDER_HPP
#define CYLINDER_HPP
#include "intersection.hpp"
#include "solid_reorientable.hpp"

namespace raytracing {
namespace Imager {
struct Vector;

/* A circular, right cylinder (may be used to make "tin can" shapes, discs with
 * thickness, etc.) Consists of a curved lateral surface and a top and bottom
 * disc.
 */
class Cylinder : public SolidObject_Reorientable {
public:
	Cylinder(double radius, double height);

protected:
	IntersectionList object_space_append_all_intersections(
		const Vector &vantage, const Vector &direction) const override;

	bool object_space_contains(const Vector &point) const override;

private:
	void append_disk_intersection(const Vector &vantage,
								  const Vector &direction, double z_disk,
								  IntersectionList &intersection_list) const;

	const double RADIUS;
	const double HALF_HEIGHT;
};

} // namespace Imager
} // namespace raytracing
#endif /* ifndef CYLINDER_HPP */
