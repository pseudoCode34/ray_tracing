#ifndef SPHEROID_HPP
#define SPHEROID_HPP

#include "intersection.hpp"
#include "solid_reorientable.hpp"

namespace raytracing {
namespace Imager {
struct Vector;

// A sphere-like object, only with different dimensions allowed in the x, y, and
// z directions.
class Spheroid : public SolidObject_Reorientable {
public:
	Spheroid(double x_axis_radius, double y_axis_radius, double z_axis_radius);

protected:
	IntersectionList object_space_append_all_intersections(
		const Vector &vantage, const Vector &direction) const override;

	bool object_space_contains(const Vector &point) const override;

private:
	const double X_AXIS_RADIUS;
	const double Y_AXIS_RADIUS;
	const double Z_AXIS_RADIUS;
	// cached for efficiency
	const double X_AXIS_RADIUS_SQR;
	const double Y_AXIS_RADIUS_SQR;
	const double Z_AXIS_RADIUS_SQR;
};
} // namespace Imager
} // namespace raytracing
#endif /* ifndef SPHEROID_HPP */
