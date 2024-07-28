#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <mp-units/systems/si/units.h>
#include <mp-units/quantity.h>
#include <mp-units/quantity_spec.h>
#include <mp-units/systems/isq/space_and_time.h>
#include <mp-units/unit.h>
#include <optional>

#include "spatial/transformable/solid_object.hpp"
#include "spatial/primitive.hpp"

namespace raytracing {
class Material;

// A solid_object that is more efficient than Spheroid with equal dimensions.
class Sphere : public SolidObject {
public:
	using RadiusInMetre
		= mp_units::quantity<mp_units::isq::radius[mp_units::si::metre], float>;
	explicit Sphere(Point3fConstRef center, const Material &uniform_optics,
					RadiusInMetre radius);

	/**
	 * \brief Returns the closest intersection of this solid_object with the
	 * specified ray if possible.
	 *
	 * \param ray
	 *
	 * Solve the quadratic equation to find all possible intersection points. If
	 * found any roots, select the smallest positive one.
	 */
	[[nodiscard]] std::optional<float>
	hit(const Eigen::ParametrizedLine<float, 3> &ray) const override;

	/**
	 * \brief Returns true if the squared distance from the center to \a point
	 * is within the square of the radius.
	 */
	[[nodiscard]] bool contains(Point3fConstRef point) const override;

	[[nodiscard]] Vector3Df normal_at(Point3fConstRef point) const override;


	[[nodiscard]] float radius2() const;


	[[nodiscard]] Vector3Df displacement_to(Vector3fConstRef point) const;


private:
	RadiusInMetre radius_;
};

} // namespace raytracing

#endif /* ifndef SPHERE_HPP */
