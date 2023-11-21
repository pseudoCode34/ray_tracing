#ifndef OPTICAL_CONSTANTS_HPP
#define OPTICAL_CONSTANTS_HPP

namespace raytracing {
namespace optical {
// A limit to how deeply in recursion CalculateLighting may go before it gives
// up, so as to avoid call stack overflow.
constexpr int RECURSION_LIMIT = 20;

/*
 * \brief A limit to how weak the red, green, or blue intensity of a light ray
 * may be after recursive calls from multiple reflections and/or refractions
 * before giving up.
 *
 * This intensity is deemed too weak to make a significant difference to the
 * image.
 */
constexpr double MIN_INTENSITY = 0.001;
} // namespace optical
} // namespace raytracing
#endif /* ifndef OPTICAL_CONSTANTS_HPP */
