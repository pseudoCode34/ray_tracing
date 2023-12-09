#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <expected>

#include "intersection.hpp"
#include "solid_object.hpp"

namespace raytracing::Imager {
struct Vector;

// A sphere that is more efficient than Spheroid with equal dimensions.
class Sphere : public SolidObject {
public:
	Sphere();
	Sphere(Vector &&center, double radius);
	Sphere(const Vector &center, double radius);

	/*
	 * \brief Determine the number of intersection where a particular direction
	 * from a given vantage point passes through the front of the Sphere at one
	 * point and emerges from the back of the Sphere at another point. In this
	 * case, it inserts two extra Intersection structs at the back of
	 * intersectionList.
	 */
	IntersectionList
	append_all_intersections(const Vector &vantage,
							 const Vector &direction) const override;

	std::expected<bool, ContainmentError>
	contains(const Vector &point) const override;

	// The nice thing about a sphere is that rotating it has no effect on its
	// appearance!
	SolidObject &rotate(double angle_in_degrees, char axis) override;

private:
	double radius_;
};

} // namespace raytracing::Imager

#endif /* ifndef SOLID_OBJECT_HPP */
