#ifndef TRANSFORMABLE_HPP
#define TRANSFORMABLE_HPP

#include "quantity.hpp"

namespace raytracing {
enum class Axis { X, Y, Z };

class SolidObject;

class Transformable {
public:
	virtual ~Transformable() = default;

	// Rotate counterclockwise around \param axis by \param angle in degree
	virtual SolidObject &rotate(DegreeAnglef angle, Axis axis) = 0;

	// Moves by the \param displacement
	virtual SolidObject &translate(Vector3fConstRef displacement) = 0;
};
} // namespace raytracing
#endif /* ifndef TRANSFORMABLE_HPP */
