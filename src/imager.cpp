#include "imager.hpp"

#include <fmt/format.h>

namespace raytracing {
#ifndef RAYTRACE_DEBUG_POINTS
#define RAYTRACE_DEBUG_POINTS 0
#endif

namespace Imager {

void indent(int recursion_depth) { fmt::print("{:>{}}", "", recursion_depth); }

} // namespace Imager
} // namespace raytracing
