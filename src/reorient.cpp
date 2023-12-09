#include "algebra.hpp"
#include "solid_reorientable.hpp"

namespace raytracing {
namespace Imager {
SolidObject_Reorientable::SolidObject_Reorientable(const Vector &center)
	: SolidObject(center),
	  r_dir_{1.0, 0.0, 0.0},
	  s_dir_{0.0, 1.0, 0.0},
	  t_dir_{0.0, 0.0, 1.0},
	  x_dir_{1.0, 0.0, 0.0},
	  y_dir_{0.0, 1.0, 0.0},
	  z_dir_{0.0, 0.0, 1.0} {}

std::expected<bool, ContainmentError>
SolidObject_Reorientable::contains(const Vector &point) const {
	return object_space_contains(object_point_from_camera_point(point));
}

Optics SolidObject_Reorientable::surface_optics(const Vector &surface_point,
												const void *context) const {
	return object_space_surface_optics(
		object_point_from_camera_point(surface_point),
		context);
}

Optics SolidObject_Reorientable::object_space_surface_optics(
	const Vector &surface_point, const void *context) const {
	return get_uniform_optics();
}

Vector SolidObject_Reorientable::object_dir_from_camera_dir(
	const Vector &camera_dir) const {
	return {camera_dir.dot(r_dir_),
			camera_dir.dot(s_dir_),
			camera_dir.dot(t_dir_)};
}

Vector SolidObject_Reorientable::object_point_from_camera_point(
	const Vector &camera_point) const {
	return object_dir_from_camera_dir(camera_point - get_center());
}

Vector SolidObject_Reorientable::camera_dir_from_object_dir(
	const Vector &object_dir) const {
	return {object_dir.dot(x_dir_),
			object_dir.dot(y_dir_),
			object_dir.dot(z_dir_)};
}

Vector SolidObject_Reorientable::camera_point_from_object_point(
	const Vector &object_point) const {
	return get_center() + camera_dir_from_object_dir(object_point);
}

void SolidObject_Reorientable::update_inverse_rotation() {
	// See the following Wikipedia articles to explain why
	// the inverse of a rotation matrix is just its transpose.
	// http://en.wikipedia.org/wiki/Rotation_matrix
	// http://en.wikipedia.org/wiki/Orthogonal_matrix

	x_dir_ = Vector{r_dir_.x, s_dir_.x, t_dir_.x};
	y_dir_ = Vector{r_dir_.y, s_dir_.y, t_dir_.y};
	z_dir_ = Vector{r_dir_.z, s_dir_.z, t_dir_.z};
}

const Vector &SolidObject::get_center() const { return center_; }

void SolidObject::set_uniform_optics(const Optics &optics) {
	uniform_optics_ = optics;
}

// Rotates counterclockwise around center looking into axis parallel to x-axis.
SolidObject &SolidObject_Reorientable::rotate(double angle_in_degrees,
											  char axis) {
	const auto [cos_val, sin_val]
		= Algebra::calculate_cos_sin(angle_in_degrees);

	switch (axis) {
	case 'x':
		r_dir_ = Vector{r_dir_.x,
						cos_val * r_dir_.y - sin_val * r_dir_.z,
						cos_val * r_dir_.z + sin_val * r_dir_.y};
		s_dir_ = Vector{s_dir_.x,
						cos_val * s_dir_.y - sin_val * s_dir_.z,
						cos_val * s_dir_.z + sin_val * s_dir_.y};
		t_dir_ = Vector{t_dir_.x,
						cos_val * t_dir_.y - sin_val * t_dir_.z,
						cos_val * t_dir_.z + sin_val * t_dir_.y};
		break;
	case 'y':
		r_dir_ = Vector{cos_val * r_dir_.x + sin_val * r_dir_.z,
						r_dir_.y,
						cos_val * r_dir_.z - sin_val * r_dir_.x};
		s_dir_ = Vector{cos_val * s_dir_.x + sin_val * s_dir_.z,
						s_dir_.y,
						cos_val * s_dir_.z - sin_val * s_dir_.x};
		t_dir_ = Vector{cos_val * t_dir_.x + sin_val * t_dir_.z,
						t_dir_.y,
						cos_val * t_dir_.z - sin_val * t_dir_.x};
		break;
	case 'z':
		r_dir_ = Vector{cos_val * r_dir_.x - sin_val * r_dir_.y,
						cos_val * r_dir_.y + sin_val * r_dir_.x,
						r_dir_.z};
		s_dir_ = Vector{cos_val * s_dir_.x - sin_val * s_dir_.y,
						cos_val * s_dir_.y + sin_val * s_dir_.x,
						s_dir_.z};
		t_dir_ = Vector{cos_val * t_dir_.x - sin_val * t_dir_.y,
						cos_val * t_dir_.y + sin_val * t_dir_.x,
						t_dir_.z};
		break;
	default:
		// FIXME: Consider a throw here
		fmt::println(stderr, "Invalid axis specified.");
	}
	update_inverse_rotation();
	return *this;
}

// Appends to 'intersectionList' a list of all the intersections
// found starting at the specified vantage point in the specified direction.
IntersectionList SolidObject_Reorientable::append_all_intersections(
	const Vector &vantage, const Vector &direction) const {
	const Vector OBJECT_VANTAGE = object_point_from_camera_point(vantage);
	const Vector OBJECT_RAY     = object_dir_from_camera_dir(direction);

	// FIXME: Logic changed when removed the loop for existing intersections
	/* const size_t SIZE_BEFORE_APPEND = intersection_list.size(); */
	IntersectionList intersection_list
		= object_space_append_all_intersections(OBJECT_VANTAGE, OBJECT_RAY);

	// Iterate only through the items we just appended, skipping over anything
	// that was already in the list before this function was called.
	for (auto &&intersection : intersection_list
		 //| std::views::drop(SIZE_BEFORE_APPEND)
	) {
		// Need to transform intersection back into camera space.
		intersection.point = camera_point_from_object_point(intersection.point);

		intersection.surface_normal
			= camera_dir_from_object_dir(intersection.surface_normal);
	}
	return intersection_list;
}
} // namespace Imager
} // namespace raytracing
