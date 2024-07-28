#include "solid_object.hpp"

namespace raytracing {
/*Ray3D SolidObject::camera_to_object_coordinates(const Ray3D &camera) const {
	return {model_matrix_.inverse() * (camera.origin - center_),
			model_matrix_.inverse().linear() * camera.direction};
}

SolidObject &SolidObject::rotate(DegreeAnglef angle, Axis axis) {
	using mp_units::angular::unit_symbols::rad;
	const auto unit_axis_vector = Vector3Df::Unit(static_cast<int>(axis));
	model_matrix_.rotate(
		Eigen::AngleAxisf(angle.numerical_value_in(rad), unit_axis_vector));
	return *this;
}

*/
void SolidObject::translate_by(Vector3fConstRef displacement) {
	model_matrix_.translate(displacement);
}

void SolidObject::rotate_by(const Eigen::Quaternionf &quaternion) {
	model_matrix_.rotate(quaternion);
}

void SolidObject::apply() {
	// Apply all chaining affine transformations to a Point3f object, not
	// Vector3f
	center_ = model_matrix_ * center_;
}

Vector3Df SolidObject::displacement_to(Vector3fConstRef point) const {
	return point - center_;
}

const Material &SolidObject::get_optics() const { return optics_; }

Material &SolidObject::get_optics() { return optics_; }

const Point3Df &SolidObject::center() const { return center_; }

/*Ray3D SolidObject::object_to_camera_coordinates(
	Point3fConstRef origin, Vector3fConstRef direction) const {
	return {model_matrix_ * (origin + center_),
			model_matrix_.linear() * direction};
}

Ray3D SolidObject::object_to_camera_coordinates(const Ray3D &object) const {
	return {model_matrix_ * (object.origin + center_),
			model_matrix_.linear() * object.direction};
}*/
void SolidObject::uniformly_scale_by(float factor) {
	model_matrix_.scale(factor);
}

void SolidObject::scale_by(Vector3fConstRef vector) {
	model_matrix_.scale(vector);
}
} // namespace raytracing
