#include "color.hpp"

#include <numeric>
#include <spdlog/sinks/basic_file_sink.h>

namespace raytracing {
namespace Imager {
/* auto color_logger = spdlog::basic_logger_mt("color", "logs/color.txt"); */
Color::Color(std::string_view color_name, double alpha) : alpha(alpha) {
	assert(alpha >= 0 && alpha <= 1);
	const auto *it = std::ranges::find_if(
		COLOR_MAPPINGS,
		[color_name](std::string_view name) { return name == color_name; },
		&ColorMapping::name);

	assert(it != COLOR_MAPPINGS.end() && "Color name is not defined.");
	red   = it->rgb[0];
	green = it->rgb[1];
	blue  = it->rgb[2];
}

Color::Color(int red, int green, int blue, double alpha)
	: red{red}, green{green}, blue{blue}, alpha{alpha} {
	validate();
}

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
