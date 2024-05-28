#include "ray.hpp"

#include <Eigen/Geometry>

namespace raytracing {
Point3f Ray::at(float t) const { return origin + direction * t; }

bool Ray::contains(Point3fConstRef point) const {
	return direction.cross(point - origin).isZero();
}

Ray standard_form_of(Point3fConstRef from, Point3fConstRef to,
					 bool need_normalize) {
	Vector3f direction = (to - from);
	if (need_normalize) direction.normalize();

	return {from, direction};
}
} // namespace raytracing
