#include "binary.hpp"

#include "vector.hpp"

#include <cmath>

namespace raytracing {
namespace Imager {
// All rotations and translations are applied to the two nested solids in
// tandem.

SolidObject &SolidObject_BinaryOperator::translate(double dx, double dy,
												   double dz) {
	SolidObject::translate(dx, dy, dz); // chain to base class
	left().translate(dx, dy, dz);       // translate left solid
	right().translate(dx, dy, dz);      // translate right solid
	return *this;
}

// rotates counterclockwise around center looking into axis parallel to x-axis.
SolidObject &SolidObject_BinaryOperator::rotate(double angle_in_degrees,
												char axis) {
	const auto TRIG_VALUES = Algebra::calculate_cos_sin(angle_in_degrees);

	nested_rotate(left(), angle_in_degrees, TRIG_VALUES, axis);
	nested_rotate(right(), angle_in_degrees, TRIG_VALUES, axis);

	return *this;
}

void SolidObject_BinaryOperator::nested_rotate(SolidObject &nested,
											   double angle_in_degrees,
											   const Algebra::TrigValues &trig,
											   char axis) {
	const auto [cos_val, sin_val] = trig;
	// Rotate the nested object about its own center.
	nested.rotate(angle_in_degrees, axis);

	// Revolve the center of the nested object around the common center of this
	// binary operator.
	const Vector &c         = get_center();
	const Vector &nc        = nested.get_center();
	const auto [dx, dy, dz] = nc - c;
	switch (axis) {
	case 'x':
		nested.move_point_to(nc.x,
							 c.y + (cos_val * dy - sin_val * dz),
							 c.z + (cos_val * dz + sin_val * dy));
		break;
	case 'y':

		nested.move_point_to(c.x + (cos_val * dx + sin_val * dz),
							 nc.y,
							 c.z + (cos_val * dz - sin_val * dx));
		break;
	case 'z':
		nested.move_point_to(c.x + (cos_val * dx - sin_val * dy),
							 c.y + (cos_val * dy + sin_val * dx),
							 nc.z);

		break;
	default:
		// FIXME: Consider a throw here
		fmt::println(stderr, "Invalid axis specified.");
	}
}
} // namespace Imager
} // namespace raytracing
