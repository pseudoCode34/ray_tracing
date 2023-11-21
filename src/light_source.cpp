#include "light_source.hpp"

namespace raytracing {
namespace Imager {
LightSource::LightSource(Vector &&location, Color &&color)
	: location(std::move(location)), color(std::move(color)) {}
} // namespace Imager
} // namespace raytracing
