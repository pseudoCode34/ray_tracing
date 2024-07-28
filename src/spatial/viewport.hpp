#ifndef VIEWPORT_HPP
#define VIEWPORT_HPP

#include "primitive.hpp"

namespace raytracing {
class [[nodiscard]] Viewport {
public:
	// NOLINTBEGIN(modernize-pass-by-value)
	explicit Viewport(Eigen::Ref<const Eigen::Matrix<float, 3, 2>> delta_uv,
					  Point3fConstRef pixel00_loc);
	// NOLINTEND(modernize-pass-by-value)

	[[nodiscard]] Point3Df at(Eigen::Index i, Eigen::Index j) const;

private:
	Eigen::Matrix<float, 3, 2> delta_uv_;
	Point3Df pixel00_loc_;
};

} // namespace raytracing
#endif /* ifndef VIEWPORT_HPP */
