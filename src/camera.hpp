#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "quantity.hpp"
#include "rect.hpp"
#include "viewport.hpp"

#include <Eigen/Geometry>

namespace raytracing {

class Camera {
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
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

	void perspective(DegreeAnglef field_of_view, float aspect_ratio,
					 float near_plane, float far_plane);

	void ortho(float left, float right, float bottom, float top,
			   float near_plane, float far_plane);

	void set_defocus_angle(DegreeAnglef defocus_angle);

	[[nodiscard]] Viewport set_viewport(Rect image) const;

	/**
	 * \note As these vector are column-major, the view matrix need to do
	 * inversion to achieve the same effect as row-major
	 *
	 * \sa glm::lookAt()
	 */
	void update_view_matrix() const;

	[[nodiscard]] Point3f orig() const;

private:
	Point3f eye_       = -Point3f::UnitZ(); // Point camera is looking from
	Point3f center_    = Point3f::Zero();   // Point camera is looking at
	Vector3f world_up_ = Vector3f::UnitY(); // Camera-relative "up" direction

	mutable Eigen::Projective3f projection_matrix_
		= Eigen::Projective3f::Identity();
	mutable Eigen::AffineCompact3f view_matrix_
		= Eigen::AffineCompact3f::Identity();

	mutable bool view_is_uptodate_ = true;

	DegreeAnglef field_of_view_ = 90 * mp_units::angular::degree;

	// Variation angle of rays through each pixel
	DegreeAnglef defocus_angle_ = 0.6 * mp_units::angular::degree;
	float near_dist_            = 0.1f;
	float far_dist_ = 10.f; // Distance from camera look from point to plane
							// of perfect focus

	ProjectionType projection_type_ = ProjectionType::PERSPECTIVE;
};

} // namespace raytracing
#endif /* ifndef CAMERA_HPP */
