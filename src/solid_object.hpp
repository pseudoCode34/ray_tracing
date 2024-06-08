#ifndef SOLID_OBJECT_HPP
#define SOLID_OBJECT_HPP

#include "material.hpp"
#include "ray.hpp"
#include "transformable.hpp"

#include <Eigen/Geometry>

namespace raytracing {
class SolidObject : public Transformable {
public:
	SolidObject(const SolidObject &)            = default;
	SolidObject(SolidObject &&)                 = delete;
	SolidObject &operator=(const SolidObject &) = default;
	SolidObject &operator=(SolidObject &&)      = delete;
	~SolidObject() override                     = default;

	// NOLINTNEXTLINE
	explicit SolidObject(Point3fConstRef center, const Material &optics)

		: center_(center), optics_(optics) {}

	/**
	 * \brief Returns the intersection at minimal distance from \a ray if
	 * possible
	 */
	[[nodiscard]] virtual float hit(const Ray &ray) const = 0;

	/**
	 * \brief Returns true if the given point is inside this solid object.
	 */
	[[nodiscard]] virtual bool contains(Point3fConstRef point) const = 0;

	[[nodiscard]] virtual Vector3f normal_at(Point3fConstRef point) const = 0;

	[[nodiscard]] Ray camera_to_object_coordinates(const Ray &camera) const;

	[[nodiscard]] Ray
	object_to_camera_coordinates(Point3fConstRef origin,
								 Vector3fConstRef direction) const;

	[[nodiscard]] Ray object_to_camera_coordinates(const Ray &object) const;

	SolidObject &rotate(DegreeAnglef angle, Axis axis) final;

	SolidObject &translate(Vector3fConstRef displacement) final;

	void apply();

	[[nodiscard]] Vector3f displacement_to(Vector3fConstRef point) const;

	[[nodiscard]] const Material &get_optics() const;

	[[nodiscard]] Material &get_optics();

	[[nodiscard]] const Point3f &center() const;

private:
	Point3f center_;

	// The point in space about which this object rotates.
	Eigen::AffineCompact3f model_matrix_ = Eigen::AffineCompact3f::Identity();
	Material optics_;
};

} // namespace raytracing

#endif /*ifndef SOLID_OBJECT_HPP*/
