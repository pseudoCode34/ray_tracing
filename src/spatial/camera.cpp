#include "camera.hpp"

#include <mp-units/compare.h>

namespace raytracing {
Camera &Camera::set_position(Point3fConstRef eye) {
	eye_              = eye;
	view_is_uptodate_ = false;
	return *this;
}

Camera &Camera::set_view_center(Point3fConstRef center) {
	center_           = center;
	view_is_uptodate_ = false;
	return *this;
}

Camera &Camera::set_world_up(Vector3fConstRef world_up) {
	world_up_         = world_up;
	view_is_uptodate_ = false;
	return *this;
}

const Eigen::AffineCompact3f &Camera::get_view_matrix() const {
	return view_matrix_;
}

void Camera::ortho(float left, float right, float bottom, float top,
				   float near_plane, float far_plane) {
	projection_type_ = ProjectionType::ORTHOGONAL;

	projection_matrix_.setIdentity();
	projection_matrix_(0, 0) = 2 / (right - left);
	projection_matrix_(1, 1) = 2 / (top - bottom);
	projection_matrix_(2, 2) = 2 / (far_plane - near_plane);
	projection_matrix_(3, 0) = -(right + left) / (right - left);
	projection_matrix_(3, 1) = -(top + bottom) / (top - bottom);
	projection_matrix_(3, 2)
		= -(far_plane + near_plane) / (far_plane - near_plane);
}

void Camera::update_view_matrix() const {
	if (view_is_uptodate_) return;

	Vector3Df forward = (eye_ - center_).normalized();
	Vector3Df right   = world_up_.cross(forward).normalized();
	Vector3Df up      = forward.cross(right);

	view_matrix_.setIdentity();
	view_matrix_.linear() << right, up, forward;
	view_matrix_.pretranslate(eye_);

	view_is_uptodate_ = true;
}

Point3Df Camera::orig() const {
	if (bool is_looking_forward = mp_units::is_lteq_zero(defocus_angle_);
		is_looking_forward)
		return eye_;

	gsl_Expects(view_is_uptodate_);
	// Returns a random point in the camera defocus disk.
	const float defocus_radius = far_dist_ * tan(defocus_angle_ / 2);
	const Eigen::Matrix<float, 3, 2> defocus_disk
		= view_matrix_.linear().leftCols(2) * defocus_radius;

	const Eigen::Vector2f offset{0.5, 0.5};
	return defocus_disk * offset + eye_;
}

const Eigen::Projective3f &Camera::get_projection_matrix() const {
	return projection_matrix_;
}

void Camera::set_defocus_angle(DegreeAnglef defocus_angle) {
	defocus_angle_ = defocus_angle;
}
} // namespace raytracing
