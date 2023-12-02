#ifndef __DDC_IMAGER_H
#define __DDC_IMAGER_H

#include "color.hpp"


namespace raytracing {
namespace Imager {
// The information available for any pixel in an ImageBuffer
struct PixelData {
	Color color;
	bool is_ambiguous{};
};

void indent(int recursion_depth);
} // namespace Imager

} // namespace raytracing
#endif // __DDC_IMAGER_H
