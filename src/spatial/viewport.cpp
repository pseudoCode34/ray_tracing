#include "viewport.hpp"

#include "spatial/primitive.hpp"

namespace raytracing {

Viewport::Viewport(Eigen::Ref<const Eigen::Matrix<float, 3, 2>> delta_uv,
				   Point3fConstRef pixel00_loc)
	: delta_uv_(delta_uv), pixel00_loc_(pixel00_loc) {}

Point3Df Viewport::at(Eigen::Index i, Eigen::Index j) const {
	return delta_uv_ * Eigen::Vector2f{i, -j} + pixel00_loc_;
}

} // namespace raytracing
