#ifndef COLOR_HPP
#define COLOR_HPP
#include "vector3f.hpp"

#include <fmt/os.h>

namespace raytracing {
using ScaledColor         = Vector3f;
using ScaledColorConstRef = const Eigen::Ref<const ScaledColor>;
using ScaledColorRef      = Eigen::Ref<ScaledColor>;
using Color               = Eigen::Matrix<uint8_t, 3, 1>;

/**
 * \brief Convert a floating point color component value, based on the maximum
 * component value, to a byte RGB value in the range 0x00 to 0xff.
 */
constexpr uint8_t convert_pixel_value(float color_component, float max) {
	auto pixel_value = static_cast<int>(255.f * color_component / max);

	return std::clamp(pixel_value, 0, UINT8_MAX);
}

/**
 * \brief Writes the gamma corrected color to file with ppm extension
 *
 * \param screen_gamma 1 incidcates no gamma correction
 *
 * \note Clamp the component value first before converting Float RGB(0-1 RGB) to
 * 8-bit channel representation
 *
 * \warnning Presume that this function be called only once before writing to
 *the image file
 */
Color to_rgb(ScaledColorConstRef float_rgb, float screen_gamma);

/**
 */
void print(fmt::ostream &os, ScaledColorConstRef color, float screen_gamma);
} // namespace raytracing

#endif
