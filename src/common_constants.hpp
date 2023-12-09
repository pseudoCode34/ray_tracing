#ifndef COMMON_CONSTANTS_HPP
#define COMMON_CONSTANTS_HPP

#include <cstdint>
#include <numbers>

namespace raytracing {
using std::numbers::pi;

constexpr uint8_t MAX_COLOR_8BIT = 255;

/*
 * \brief EPSILON is a tolerance value for floating point roundoff error.
 *
 * It is used in many calculations where we want to err on a certain side of a
 * threshold, such as determining whether or not a point is inside a solid or
 * not, or whether a point is at least a minimum distance away from another
 * point.
 */
constexpr double EPSILON = 1.0e-6;

constexpr double radian_from_degree(double degree) {
	constexpr double PI_RADIAN = 180.0;
	return degree * (pi / PI_RADIAN);
}
} // namespace raytracing
#endif /* ifndef COMMON_CONSTANTS_HPP */
