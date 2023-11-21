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
SolidObject &SolidObject_BinaryOperator::rotate_x(double angle_in_degrees) {
	const double ANGLE_IN_RADIANS = radian_from_degree(angle_in_degrees);
	const double A                = cos(ANGLE_IN_RADIANS);
	const double B                = sin(ANGLE_IN_RADIANS);

	nested_rotate_x(left(), angle_in_degrees, A, B);
	nested_rotate_x(right(), angle_in_degrees, A, B);

	return *this;
}

// rotates counterclockwise around center looking into axis parallel to y-axis.
SolidObject &SolidObject_BinaryOperator::rotate_y(double angle_in_degrees) {
	const double ANGLE_IN_RADIANS = radian_from_degree(angle_in_degrees);
	const double a                = cos(ANGLE_IN_RADIANS);
	const double b                = sin(ANGLE_IN_RADIANS);

	nested_rotate_y(left(), angle_in_degrees, a, b);
	nested_rotate_y(right(), angle_in_degrees, a, b);

	return *this;
}

// rotates counterclockwise around center looking into axis parallel to z-axis.
SolidObject &SolidObject_BinaryOperator::rotate_z(double angle_in_degrees) {
	const double ANGLE_IN_RADIANS = radian_from_degree(angle_in_degrees);
	const double a                = cos(ANGLE_IN_RADIANS);
	const double b                = sin(ANGLE_IN_RADIANS);

	nested_rotate_z(left(), angle_in_degrees, a, b);
	nested_rotate_z(right(), angle_in_degrees, a, b);

	return *this;
}

void SolidObject_BinaryOperator::nested_rotate_x(SolidObject &nested,
												 double angle_in_degrees,
												 double a, double b) {
	// Rotate the nested object about its own center.
	nested.rotate_x(angle_in_degrees);

	// Revolve the center of the nested object around the common center of this
	// binary operator.
	const Vector &c  = center();
	const Vector &nc = nested.center();
	const double dy  = nc.y - c.y;
	const double dz  = nc.z - c.z;
	nested.move_point_to(nc.x,
						 c.y + (a * dy - b * dz),
						 c.z + (a * dz + b * dy));
}

void SolidObject_BinaryOperator::nested_rotate_y(SolidObject &nested,
												 double angle_in_degrees,
												 double a, double b) {
	// Rotate the nested object about its own center.
	nested.rotate_y(angle_in_degrees);

	// Revolve the center of the nested object around the common center of this
	// binary operator.
	const Vector &c  = center();
	const Vector &nc = nested.center();
	const double dx  = nc.x - c.x;
	const double dz  = nc.z - c.z;
	nested.move_point_to(c.x + (a * dx + b * dz),
						 nc.y,
						 c.z + (a * dz - b * dx));
}

void SolidObject_BinaryOperator::nested_rotate_z(SolidObject &nested,
												 double angle_in_degrees,
												 double a, double b) {
	// Rotate the nested object about its own center.
	nested.rotate_z(angle_in_degrees);

	// Revolve the center of the nested object around the common center of this
	// binary operator.
	const Vector &c  = center();
	const Vector &nc = nested.center();
	const double dx  = nc.x - c.x;
	const double dy  = nc.y - c.y;
	nested.move_point_to(c.x + (a * dx - b * dy),
						 c.y + (a * dy + b * dx),
						 nc.z);
}

} // namespace Imager
} // namespace raytracing
