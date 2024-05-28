#include "viewport.hpp"

namespace raytracing {
Viewport::Viewport(
	const Eigen::Matrix3f &mat) // NOLINT(modernize-pass-by-value)
	: mat_(mat) {}

Viewport::Viewport(Vector3fConstRef delta_u, Vector3fConstRef delta_v,
				   Point3fConstRef pixel00_loc) {
	mat_ << delta_u, delta_v, pixel00_loc;
}

Point3f Viewport::at(float i, float j) const {
	return mat_ * Vector3f{i, j, 1};
}

} // namespace raytracing
