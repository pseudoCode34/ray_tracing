#ifndef COLOR_HPP
#define COLOR_HPP

#include "algebra.hpp"

#include <cassert>
#include <fmt/format.h>
#include <memory>
#include <spdlog/logger.h>
#include <stdexcept>
#include <stdint.h>

namespace raytracing {
namespace Imager {
struct ColorMapping {
	std::string_view name;
	std::array<int, 3> rgb;
};

struct Color {
	int red, green, blue;
	double alpha;

	/*
	 * [constructors]
	 */

	constexpr Color() : red(0), green(0), blue(0), alpha(1) {}

	Color(int red, int green, int blue, double alpha = 1)
		: red{red}, green{green}, blue{blue}, alpha{alpha} {
		validate();
	}

	explicit Color(std::string_view color_name, double alpha = 1)
		: alpha(alpha) {
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

	/*
	 * [operators]
	 */
	constexpr auto operator<=>(const Color &) const = default;

	constexpr bool operator==(const Color &other) const noexcept {
		if (red != other.red) return false;
		if (green != other.green) return false;
		if (blue != other.blue) return false;
		if (!Algebra::is_equal(alpha, other.alpha)) return false;
		return true;
	}

	Color &operator+=(const Color &other);
	friend Color operator+(Color lhs, const Color &rhs);

	Color &operator+=(double other);
	friend Color operator+(Color color, double factor);
	friend Color operator+(double factor, Color color);

	Color &operator*=(const Color &other);
	friend Color operator*(Color lhs, const Color &rhs);

	Color &operator*=(double factor);
	friend Color operator*(Color color, double factor);
	friend Color operator*(double factor, Color color);

	Color &operator/=(double other);
	friend Color operator/(Color lhs, double rhs);

	/**
	 * \brief Assert that Red, Green, Blue colorspace is within the range of
	 * [0..255] and alpha is within the range of [0..1].
	 *
	 * A color is valid for reflection if all its components are in the range
	 * 0..255. Otherwise, it is possible for multiple reflections to keep
	 * "amplifying" light beyond any limit.
	 */
	void validate() const {
		assert(red >= 0 && red <= 255);
		assert(green >= 0 && green <= 255);
		assert(blue >= 0 && blue <= 255);
		assert(alpha >= 0 && alpha <= 1);
	}

private:
	static constexpr std::array<ColorMapping, 6> COLOR_MAPPINGS = {{
		{"black", {0, 0, 0}},
		{"white", {255, 255, 255}},
		{"red", {255, 0, 0}},
		{"green", {0, 255, 0}},
		{"blue", {0, 0, 255}},
		{"grey", {0, 0, 255}},
	}};
};

extern std::shared_ptr<spdlog::logger> color_logger;
} // namespace Imager

} // namespace raytracing

template <>
struct fmt::formatter<raytracing::Imager::Color> {
	constexpr auto parse(fmt::format_parse_context &ctx) {
		const auto *pos = ctx.begin();
		while (pos != ctx.end() && *pos != '}') {
			if (*pos == 'h' || *pos == 'H') is_hex = true;
			++pos;
		}
		return pos; // expect `}` at this position, otherwise, it's error!
	}

	auto format(const raytracing::Imager::Color &col,
				fmt::format_context &ctx) {
		const auto [red, green, blue, alpha] = col;
		if (is_hex) {
			uint32_t val = static_cast<uint8_t>(red) << 16
						   | static_cast<uint8_t>(green) << 8
						   | static_cast<uint8_t>(blue);
			return fmt::format_to(ctx.out(), "#{:x}", val);
		}

		return fmt::format_to(ctx.out(), "({}, {}, {})", red, green, blue);
	}

	bool is_hex{false};
};
#endif
