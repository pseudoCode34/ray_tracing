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

	[[nodiscard]] Ray camera_to_object_coordinates(const Ray &camera) const {
		return {model_matrix_.inverse() * (camera.origin - center_),
				model_matrix_.inverse().linear() * camera.direction};
	}

	[[nodiscard]] Ray
	object_to_camera_coordinates(Point3fConstRef origin,
								 Vector3fConstRef direction) const {
		return {model_matrix_ * (origin + center_),
				model_matrix_.linear() * direction};
	}

	[[nodiscard]] Ray object_to_camera_coordinates(const Ray &object) const {
		return {model_matrix_ * (object.origin + center_),
				model_matrix_.linear() * object.direction};
	}

	SolidObject &rotate(DegreeAnglef angle, Axis axis) final {
		using mp_units::angular::unit_symbols::rad;
		const auto unit_axis_vector = Vector3f::Unit(static_cast<int>(axis));
		model_matrix_.rotate(
			Eigen::AngleAxisf(angle.numerical_value_in(rad), unit_axis_vector));
		return *this;
	}

	SolidObject &translate(Vector3fConstRef displacement) final {
		model_matrix_.translate(displacement);
		return *this;
	}

	void apply() {
		// Apply all chaining affine transformations to a Point3f object, not
		// Vector3f
		center_ = model_matrix_ * center_;
	}

	[[nodiscard]] Vector3f displacement_to(Vector3fConstRef point) const {
		return point - center_;
	}

	[[nodiscard]] const Material &get_optics() const { return optics_; }

	[[nodiscard]] Material &get_optics() { return optics_; }

private:
	Point3f center_;

	// The point in space about which this object rotates.
	Eigen::AffineCompact3f model_matrix_ = Eigen::AffineCompact3f::Identity();
	Material optics_;
};

} // namespace raytracing

#endif /*ifndef SOLID_OBJECT_HPP*/
