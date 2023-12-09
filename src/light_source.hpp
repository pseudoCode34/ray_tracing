#ifndef LIGHT_SOURCE_HPP
#define LIGHT_SOURCE_HPP

#include "color.hpp"
#include "vector.hpp"

namespace raytracing::Imager {
// For now, all light sources are single points with an inherent color.
// Luminosity of the light source can be changed by multiplying color.red,
// color.green, color.blue all by a constant value.
struct LightSource {
	Vector location;
	Color color = Color(255, 255, 255);
};
} // namespace raytracing::Imager

#endif /* ifndef LIGHT_SOURCE_HPP */
