#ifndef SOLID_OBJECT_HPP
#define SOLID_OBJECT_HPP

#include "spatial/material.hpp"
#include "spatial/primitive.hpp"
#include "spatial/transformable/rotatable.hpp"
#include "spatial/transformable/scalable.hpp"
#include "spatial/transformable/translatable.hpp"
#include "spatial/transformable/volumetric.hpp"

namespace raytracing {
class SolidObject : public Translatable,
					public Rotatable,
					public Volumetric,
					public Scalable {
public:
	SolidObject(const SolidObject &)            = default;
	SolidObject(SolidObject &&)                 = delete;
	SolidObject &operator=(const SolidObject &) = default;
	SolidObject &operator=(SolidObject &&)      = delete;
	~SolidObject() override                     = default;

	// NOLINTNEXTLINE
	explicit SolidObject(Point3fConstRef center, const Material &optics)
		: center_(center), optics_(optics) {}

	/*[[nodiscard]] Ray3D camera_to_object_coordinates(const Ray3D &camera)
	const;

	[[nodiscard]] Ray3D
	object_to_camera_coordinates(Point3fConstRef origin,
								 Vector3fConstRef direction) const;

	[[nodiscard]] Ray3D object_to_camera_coordinates(const Ray3D &object)
	const;*/

	void translate_by(Vector3fConstRef displacement) override;

	void rotate_by(const Eigen::Quaternionf &quaternion) override;


	void scale_by(Vector3fConstRef vector) override;

	void uniformly_scale_by(float factor) override;

	void apply();

	[[nodiscard]] Vector3Df displacement_to(Vector3fConstRef point) const;

	[[nodiscard]] const Material &get_optics() const;

	[[nodiscard]] Material &get_optics();

	[[nodiscard]] const Point3Df &center() const;

private:
	Point3Df center_;

	// The point in space about which this object rotates.
	Eigen::AffineCompact3f model_matrix_ = Eigen::AffineCompact3f::Identity();
	Material optics_;
};


} // namespace raytracing

#endif /*ifndef SOLID_OBJECT_HPP*/
