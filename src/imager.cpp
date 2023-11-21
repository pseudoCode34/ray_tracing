#include "imager.hpp"

#include <fmt/format.h>

namespace raytracing {
#ifndef RAYTRACE_DEBUG_POINTS
#define RAYTRACE_DEBUG_POINTS 0
#endif

namespace Imager {
/* auto shared_imager_sink */
/* 	= std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/imager.txt"); */

ImagerException::ImagerException(const std::string &message)
	: std::runtime_error(message) // std::runtime_error will handle the string
{}

AmbiguousIntersectionException::AmbiguousIntersectionException(
	const std::string &message)
	: std::runtime_error{message} {}

void indent(int recursion_depth) { fmt::print("{:>{}}", "", recursion_depth); }

} // namespace Imager
} // namespace raytracing
