#ifndef LIGHT_SOURCE_HPP
#define LIGHT_SOURCE_HPP

#include "primitive.hpp"

namespace raytracing {
struct LightSource {
	Point3Df position;
	ScaledColor diffuse;  // Diffuse intensity of the light
	ScaledColor specular; // Specular intensity of the light
};

using LightSourceList = std::vector<LightSource>;

} // namespace raytracing

#endif /* ifndef LIGHT_SOURCE_HPP */
