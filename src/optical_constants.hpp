#ifndef OPTICAL_CONSTANTS_HPP
#define OPTICAL_CONSTANTS_HPP

namespace raytracing::optical {
/*
 * \brief A limit to how weak the red, green, or blue component of a light ray
 * may be after recursive calls from multiple reflections and/or refractions
 * before giving up.
 *
 * This intensity is deemed too weak to make a significant difference to the
 * image.
 */
constexpr int MIN_RGB_COMPONENT = 10;
constexpr float MAX_INTENSITY   = 0.99;
constexpr float MIN_INTENSITY   = 0.01;

} // namespace raytracing::optical

#endif /* ifndef OPTICAL_CONSTANTS_HPP */
