#include "color.hpp"

namespace raytracing {
Color to_rgb(ScaledColorConstRef float_rgb, float screen_gamma) {
	return (float_rgb.array().pow(1 / screen_gamma).cwiseMin(1.f).cwiseMax(0.f)
			* 255)
		.cast<uint8_t>();
}

void print(fmt::ostream &os, ScaledColorConstRef color, float screen_gamma) {
	const auto final = to_rgb(color, screen_gamma);
	os.print("{} {} {}\n", final.x(), final.y(), final.z());
}
} // namespace raytracing
