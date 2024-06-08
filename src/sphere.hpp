#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "point3f.hpp"
#include "solid_object.hpp"

namespace raytracing {
class Material;
struct Ray;

// A solid_object that is more efficient than Spheroid with equal dimensions.
class Sphere : public SolidObject {
public:
	Sphere(Point3fConstRef center, const Material &uniform_optics,
		   float radius = 1);

	/**
	 * \copybrief SolidObject::hit()
	 * \brief Returns the closest intersection of this solid_object with the
	 * specified ray if possible.
	 *
	 * \param ray
	 *
	 * Solve the quadratic equation to find all possible intersection points. If
	 * found any roots, select one with the smallest positive parametric ray
	 * root.
	 */
	[[nodiscard]] float hit(const Ray &ray) const override;

	/**
	 * \brief Returns true if the square of the squared distance from the center
	 * to \param point is within the square of the radius.
	 */
	[[nodiscard]] bool contains(Point3fConstRef point) const override;

	[[nodiscard]] Vector3f normal_at(Point3fConstRef point) const override;

private:
	float radius_;
};

} // namespace raytracing

#endif /* ifndef SPHERE_HPP */
