#ifndef __DDC_IMAGER_H
#define __DDC_IMAGER_H

#include "color.hpp"

#include <stdexcept>
#include <string>

namespace raytracing {
namespace Imager {
/*
 * \brief An exception thrown when multiple intersections lie at the same
 * distance from the vantage point.
 *
 * SaveImage catches these and marks such pixels as ambiguous. It performs a
 * second pass later that averages the color values of surrounding non-ambiguous
 * pixels.
 */
struct AmbiguousIntersectionException : std::runtime_error {
	AmbiguousIntersectionException(const std::string &message);
};

// An exception thrown by imager code when a fatal error occurs.
struct ImagerException : std::runtime_error {
	ImagerException(const std::string &message);
};

// The information available for any pixel in an ImageBuffer
struct PixelData {
	Color color;
	bool is_ambiguous{};
};

void indent(int recursion_depth);
} // namespace Imager

} // namespace raytracing
#endif // __DDC_IMAGER_H
