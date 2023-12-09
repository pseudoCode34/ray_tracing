#include "imager.hpp"

#include <fmt/format.h>

namespace raytracing {
namespace Imager {

void indent(int recursion_depth) { fmt::print("{:>{}}", "", recursion_depth); }

} // namespace Imager
} // namespace raytracing
