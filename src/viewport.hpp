#ifndef VIEWPORT_HPP
#define VIEWPORT_HPP

#include "point3f.hpp"

#include <cstddef>

namespace raytracing {
class Viewport {
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

	/**
	 * \note The current Eigen3 version 3.4 doesn't support move semantics yet
	 */
	explicit Viewport(const Eigen::Matrix3f &mat);

	// NOLINTBEGIN(modernize-pass-by-value)
	explicit Viewport(Vector3fConstRef delta_u, Vector3fConstRef delta_v,
					  Point3fConstRef pixel00_loc);
	// NOLINTEND(modernize-pass-by-value)

	[[nodiscard]] Point3f at(size_t i, size_t j) const;

private:
	Eigen::Matrix3f mat_;
};

} // namespace raytracing
#endif /* ifndef VIEWPORT_HPP */
