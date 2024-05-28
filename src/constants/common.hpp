#ifndef COMMON_CONSTANTS_HPP
#define COMMON_CONSTANTS_HPP

namespace raytracing {

/*
 * \brief EPSILON is a tolerance value for floating point roundoff error.
 *
 * It is used in many calculations where we want to err on a certain side of a
 * threshold, such as determining whether a point is inside a solid or not, or
 * whether a point is at least a minimum distance away from another point.
 */
constexpr float EPSILON = 1e-4f;
} // namespace raytracing
#endif /* ifndef COMMON_CONSTANTS_HPP */
