#include "solid_object.hpp"

namespace raytracing {
Ray SolidObject::camera_to_object_coordinates(const Ray &camera) const {
	return {model_matrix_.inverse() * (camera.origin - center_),
			model_matrix_.inverse().linear() * camera.direction};
}

SolidObject &SolidObject::rotate(DegreeAnglef angle, Axis axis) {
	using mp_units::angular::unit_symbols::rad;
	const auto unit_axis_vector = Vector3f::Unit(static_cast<int>(axis));
	model_matrix_.rotate(
		Eigen::AngleAxisf(angle.numerical_value_in(rad), unit_axis_vector));
	return *this;
}

SolidObject &SolidObject::translate(Vector3fConstRef displacement) {
	model_matrix_.translate(displacement);
	return *this;
}

void SolidObject::apply() {
	// Apply all chaining affine transformations to a Point3f object, not
	// Vector3f
	center_ = model_matrix_ * center_;
}

Vector3f SolidObject::displacement_to(Vector3fConstRef point) const {
	return point - center_;
}

const Material &SolidObject::get_optics() const { return optics_; }

Material &SolidObject::get_optics() { return optics_; }

const Point3f &SolidObject::center() const { return center_; }

Ray SolidObject::object_to_camera_coordinates(
	Point3fConstRef origin, Vector3fConstRef direction) const {
	return {model_matrix_ * (origin + center_),
			model_matrix_.linear() * direction};
}

Ray SolidObject::object_to_camera_coordinates(const Ray &object) const {
	return {model_matrix_ * (object.origin + center_),
			model_matrix_.linear() * object.direction};
}
} // namespace raytracing
