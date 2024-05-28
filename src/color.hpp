#ifndef COLOR_HPP
#define COLOR_HPP
#include "vector3f.hpp"

namespace raytracing {
using ScaledColor         = Vector3f;
using ScaledColorConstRef = const Eigen::Ref<const ScaledColor>;
using ScaledColorRef      = Eigen::Ref<ScaledColor>;

/**
 * \brief Convert a floating point color component value, based on the maximum
 * component value, to a byte RGB value in the range 0x00 to 0xff.
 */
constexpr uint8_t convert_pixel_value(float color_component, float max) {
	auto pixel_value = static_cast<int>(255.f * color_component / max);

	return std::clamp(pixel_value, 0, UINT8_MAX);
}
} // namespace raytracing

template <>
struct fmt::formatter<raytracing::ScaledColor> {
	constexpr auto parse(const fmt::format_parse_context &ctx) {
		return ctx.begin();
	}

	auto format(raytracing::ScaledColorConstRef vector,
				fmt::format_context &ctx) {
		return fmt::format_to(ctx.out(),
							  "{} {} {}",
							  vector.x(),
							  vector.y(),
							  vector.z());
	}
};
#endif
