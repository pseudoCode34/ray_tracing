#ifndef LIGHT_SOURCE_HPP
#define LIGHT_SOURCE_HPP

#include "color.hpp"
#include "vector.hpp"

namespace raytracing {
namespace Imager {
// For now, all light sources are single points with an inherent color.
// Luminosity of the light source can be changed by multiplying color.red,
// color.green, color.blue all by a constant value.
struct LightSource {
	// TODO: Use move semantics here?
	// only location, or both location and color?
	LightSource(Vector &&location, Color &&color);

	Vector location;
	Color color;
};
} // namespace Imager
} // namespace raytracing
#endif /* ifndef LIGHT_SOURCE_HPP */
