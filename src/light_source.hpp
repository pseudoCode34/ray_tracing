#ifndef LIGHT_SOURCE_HPP
#define LIGHT_SOURCE_HPP

#include "color.hpp"
#include "point3f.hpp"

namespace raytracing {
// For now, all light sources are single points with an inherent color.
// Luminosity of the light source can be changed by multiplying color.red,
// color.green, color.blue all by a constant value.
struct LightSource {
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	Point3f position  = Point3f::Zero();
	ScaledColor color = ScaledColor::Ones();
};

using LightSourceList = std::vector<LightSource>;

} // namespace raytracing

#endif /* ifndef LIGHT_SOURCE_HPP */
