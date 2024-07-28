#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "quantity/angle.hpp"
#include "quantity/pixel.hpp"
#include "spatial/primitive.hpp"
#include "viewport.hpp"

#include <gsl/gsl-lite.hpp>
#include <mp-units/bits/magnitude.h>
#include <mp-units/bits/quantity_concepts.h>
#include <mp-units/quantity.h>
#include <mp-units/quantity_spec.h>
#include <mp-units/reference.h>
#include <mp-units/systems/angular/angular.h>
#include <mp-units/systems/isq/space_and_time.h>

namespace raytracing {

class Camera {
public:
	Camera()                          = default;
	Camera(const Camera &)            = default;
	Camera(Camera &&)                 = default;
	Camera &operator=(const Camera &) = default;
	Camera &operator=(Camera &&)      = default;
	~Camera() noexcept                = default;

	enum class ProjectionType { PERSPECTIVE, ORTHOGONAL };

	Camera &set_position(Point3fConstRef eye);

	Camera &set_view_center(Point3fConstRef center);

	Camera &set_world_up(Vector3fConstRef world_up);

	[[nodiscard]] const Eigen::AffineCompact3f &get_view_matrix() const;

	[[nodiscard]] const Eigen::Projective3f &get_projection_matrix() const;

	void perspective(
		mp_units::QuantityOf<mp_units::angular::angle> auto field_of_view,
		float aspect_ratio, float near_plane, float far_plane) {
		gsl_Expects(near_plane > 0);
		gsl_Expects(far_plane > 0);

		projection_type_ = ProjectionType::PERSPECTIVE;
		vfov_            = field_of_view;
		aspect_ratio_    = aspect_ratio;
		near_dist_       = near_plane;
		far_dist_        = far_plane;
		auto f           = 1.f / tan(vfov_ / 2);

		projection_matrix_.setIdentity();
		projection_matrix_(0, 0) = f / aspect_ratio;
		projection_matrix_(1, 1) = f;
		projection_matrix_(2, 2)
			= (near_plane + far_plane) / (near_plane - far_plane);
		projection_matrix_(2, 3)
			= 2 * near_plane * far_plane / (near_plane - far_plane);
		projection_matrix_(3, 2) = -1;
		projection_matrix_(3, 3) = 0;
	}

	void ortho(float left, float right, float bottom, float top,
			   float near_plane, float far_plane);

	void set_defocus_angle(DegreeAnglef defocus_angle);

	Viewport set_viewport(
		mp_units::QuantityOf<mp_units::isq::height> auto image_height) const {
		gsl_Expects(view_is_uptodate_);

		auto half_height = tan(vfov_ / 2) * far_dist_;
		auto half_width  = half_height * aspect_ratio_;

		const Eigen::Matrix<float, 3, 2> half_delta_uv
			= view_matrix_.linear().leftCols(2) * half_height
			  / image_height.numerical_value_in(pixel);
		Vector3Df pixel00_loc
			= half_delta_uv.rowwise().sum()
			  + view_matrix_.affine()
					* Eigen::Vector4f{-half_width, half_height, -far_dist_, 1};
		return Viewport{2 * half_delta_uv, pixel00_loc};
	}

	/**
	 * \note As these vector are column-major, the view matrix need to do
	 * inversion to achieve the same effect as row-major
	 *
	 * \sa glm::lookAt()
	 */
	void update_view_matrix() const;

	/**
	 * \brief The origin of th
	 */
	[[nodiscard]] Point3Df orig() const;

private:
	Point3Df eye_       = -Point3Df::UnitZ(); // Point camera is looking from
	Point3Df center_    = Point3Df::Zero();   // Point camera is looking at
	Vector3Df world_up_ = Vector3Df::UnitY(); // Camera-relative "up" direction

	mutable Eigen::Projective3f projection_matrix_;
	mutable Eigen::AffineCompact3f view_matrix_;

	mutable bool view_is_uptodate_ = true;

	// Vertical view angle (field of view)
	DegreeAnglef vfov_ = 90 * mp_units::angular::degree;

	// Variation angle of rays through each pixel
	DegreeAnglef defocus_angle_ = 0.6 * mp_units::angular::degree;
	float aspect_ratio_         = 16.f / 9.f;
	// Distance to the near clipping plane
	float near_dist_ = 0.1f;
	// Distance from camera look-from point to far clipping lane
	float far_dist_ = 10.f;

	ProjectionType projection_type_ = ProjectionType::PERSPECTIVE;
};

} // namespace raytracing
#endif /* ifndef CAMERA_HPP */
