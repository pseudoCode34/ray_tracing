#include "color.hpp"

#include "common_constants.hpp"

#include <numeric>
#include <spdlog/sinks/basic_file_sink.h>

namespace raytracing {
namespace Imager {
/* auto color_logger = spdlog::basic_logger_mt("color", "logs/color.txt"); */

Color &Color::operator+=(const Color &other) {
	red += other.red;
	green += other.green;
	blue += other.blue;
	return *this;
}

Color &Color::operator+=(double other) {
	red += other;
	green += other;
	blue += other;
	return *this;
}

Color &Color::operator*=(const Color &other) {
	red *= other.red;
	green *= other.green;
	blue *= other.blue;
	return *this;
}

Color &Color::operator*=(double factor) {
	red   = factor * red;
	green = factor * green;
	blue  = factor * blue;
	return *this;
}

Color &Color::operator/=(double other) {
	red /= other;
	green /= other;
	blue /= other;
	return *this;
}

Color operator/(Color lhs, double rhs) { return lhs /= rhs; }

Color operator*(Color lhs, const Color &rhs) {
	return lhs *= rhs / PIXEL_8BIT_MAX_COLOR;
}

Color operator*(Color color, double factor) { return color *= factor; }

Color operator*(double factor, Color color) { return color *= factor; }

Color operator+(Color lhs, const Color &rhs) { return lhs += rhs; }

Color operator+(Color color, double factor) { return color += factor; }

Color operator+(double factor, Color color) { return color += factor; }
} // namespace Imager
} // namespace raytracing
