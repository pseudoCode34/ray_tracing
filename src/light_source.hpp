#ifndef LIGHT_SOURCE_HPP
#define LIGHT_SOURCE_HPP

#include "color.hpp"
#include "point3f.hpp"

namespace raytracing {
struct LightSource {
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	Point3f position;
	ScaledColor diffuse;  // Diffuse intensity of the light
	ScaledColor specular; // Specular intensity of the light
};

using LightSourceList = std::vector<LightSource>;

} // namespace raytracing

#endif /* ifndef LIGHT_SOURCE_HPP */
