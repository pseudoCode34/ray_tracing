#ifndef SET_HPP
#define SET_HPP

#include "binary.hpp"
#include "intersection.hpp"
#include "solid_object.hpp"

#include <memory>

namespace raytracing {
namespace Imager {

class SetUnion : public SolidObject_BinaryOperator {
public:
	explicit SetUnion(const Vector &center, SolidObject *left,
					  SolidObject *right);

	IntersectionList
	append_all_intersections(const Vector &vantage,
							 const Vector &direction) const override;

	bool contains(const Vector &point) const override;
};

// Filters a SolidObject, except toggles the inside/outside property.
// For example, a sphere becomes the set of points outside the sphere.
class SetComplement : public SolidObject {
public:
	explicit SetComplement(SolidObject *other);

	bool contains(const Vector &point) const override;

	IntersectionList
	append_all_intersections(const Vector &vantage,
							 const Vector &direction) const override;

	SolidObject &translate(double dx, double dy, double dz) override;

	SolidObject &rotate_x(double angle_in_degrees) override;

	SolidObject &rotate_y(double angle_in_degrees) override;

	SolidObject &rotate_z(double angle_in_degrees) override;

private:
	std::unique_ptr<SolidObject> other_;
};

class SetIntersection : public SolidObject_BinaryOperator {
public:
	explicit SetIntersection(const Vector &center, SolidObject *left,
							 SolidObject *right);

	IntersectionList
	append_all_intersections(const Vector &vantage,
							 const Vector &direction) const override;
	bool contains(const Vector &point) const override;

private:
	IntersectionList append_overlapping_intersections(
		const Vector &vantage, const Vector &direction,
		const SolidObject &a_solid, const SolidObject &b_solid) const;

	bool has_overlapping_intersection(const Vector &vantage,
									  const Vector &direction,
									  const SolidObject &a_solid,
									  const SolidObject &b_solid) const;
};

// Set difference is a variation on set intersection:
// The difference A - B is identical to the intersection of A and not(B).
class SetDifference : public SetIntersection {
public:
	/* explicit SetDifference(const Vector &center, SolidObjectPtr &left, */
	/* 					   SolidObjectPtr &right); */
	explicit SetDifference(const Vector &center, SolidObject *left,
						   SolidObject *right);
};

} // namespace Imager
} // namespace raytracing

#endif /* ifndef SET_HPP */
