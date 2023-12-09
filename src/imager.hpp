#ifndef __DDC_IMAGER_H
#define __DDC_IMAGER_H

#include "color.hpp"

namespace raytracing::Imager {
#ifndef RAYTRACE_DEBUG_POINTS
#define RAYTRACE_DEBUG_POINTS 0
#endif
// The information available for any pixel in an ImageBuffer
struct PixelData {
	Color color;
	bool is_ambiguous{};
};

void indent(int recursion_depth);
} // namespace raytracing::Imager


#endif // __DDC_IMAGER_H
