#include "camera.hpp"

#include "floating_point_comparisons.hpp"

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

void Camera::perspective(DegreeAnglef field_of_view, float aspect_ratio,
						 float near_plane, float far_plane) {
	assert(!FPC::near_zero(near_plane));

	projection_type_ = ProjectionType::PERSPECTIVE;
	field_of_view_   = field_of_view;
	near_dist_       = near_plane;
	far_dist_        = far_plane;

	auto f = 1.f / tan(field_of_view_ / 2);

	projection_matrix_(0, 0) = f / aspect_ratio;
	projection_matrix_(1, 1) = f;
	projection_matrix_(2, 2)
		= (near_plane + far_plane) / (near_plane - far_plane);
	projection_matrix_(2, 3)
		= 2 * near_plane * far_plane / (near_plane - far_plane);
	projection_matrix_(3, 2) = -1;
	projection_matrix_(3, 3) = 0;
}

void Camera::ortho(float left, float right, float bottom, float top,
				   float near_plane, float far_plane) {
	projection_type_ = ProjectionType::ORTHOGONAL;

	projection_matrix_(0, 0) = 2 / (right - left);
	projection_matrix_(1, 1) = 2 / (top - bottom);
	projection_matrix_(2, 2) = 2 / (far_plane - near_plane);
	projection_matrix_(3, 0) = -(right + left) / (right - left);
	projection_matrix_(3, 1) = -(top + bottom) / (top - bottom);
	projection_matrix_(3, 2)
		= -(far_plane + near_plane) / (far_plane - near_plane);
}

Viewport Camera::set_viewport(Rect image) const {
	using Eigen::Vector4f;
	auto height = 2 * tan(field_of_view_ / 2) * far_dist_;
	auto width  = image.aspect_ratio() * height;
	auto z      = height / image.height;

	Eigen::Matrix3f res;
	Vector3f delta_u = view_matrix_.linear().col(0) * z;
	Vector3f delta_v = view_matrix_.linear().col(1) * -z;
	Vector3f pixel00_loc
		= (delta_u + delta_v) / 2
		  + view_matrix_.affine()
				* Vector4f{-width / 2, height / 2, -far_dist_, 1};
	res << delta_u, delta_v, pixel00_loc;
	return res;
}

void Camera::update_view_matrix() const {
	if (view_is_uptodate_) return;

	Vector3f f = (eye_ - center_).normalized();
	Vector3f r = world_up_.cross(f).normalized();
	Vector3f u = f.cross(r);

	view_matrix_.linear() << r, u, f;
	view_matrix_.pretranslate(eye_);

	view_is_uptodate_ = true;
}

Point3f Camera::orig() const {
	if (bool is_looking_forward = mp_units::is_lteq_zero(defocus_angle_);
		is_looking_forward)
		return eye_;

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
