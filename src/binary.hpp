#ifndef SOLID_OBJECT_BINARY
#define SOLID_OBJECT_BINARY
#include "solid_object.hpp"
#include <memory>

namespace raytracing {
namespace Imager {
// This class encapsulates the notion of a binary operator that operates on two
// SolidObjects. Both SolidObjects must support the Contains() method, or an
// exception will occur during rendering.
class SolidObject_BinaryOperator : public SolidObject {
public:
	// The parameters '_left' and '_right' must be dynamically
	// allocated using operator new. This class will own
	// responsibility for deleting them when it is itself deleted.
	SolidObject_BinaryOperator(const Vector &center, SolidObject *left,
							   SolidObject *right);

	// All rotations and translations are applied
	// to the two nested solids in tandem.

	// The following three member functions rotate this object counterclockwise
	// around a line parallel to the x, y, or z axis, as seen from the positive
	// axis direction.
	SolidObject &rotate(double angle_in_degrees, char axis) override;

	SolidObject &translate(double dx, double dy, double dz) override;

protected:
	SolidObject &left() const;

	SolidObject &right() const;

	void nested_rotate(SolidObject &nested, double angle_in_degrees,
					   const Algebra::TrigValues &trig, char axis);
	// The following list is for caching and filtering
	// intersections with the left and right nested solids.
	// It is mutable to allow modification from const methods.
	mutable IntersectionList temp_intersection_list;

private:
	std::shared_ptr<SolidObject> left_, right_;
};

} // namespace Imager
} // namespace raytracing
#endif /* ifndef SOLID_OBJECT_BINARY */
