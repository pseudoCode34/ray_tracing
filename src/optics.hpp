#ifndef OPTICS_HPP
#define OPTICS_HPP

#include "color.hpp"

namespace raytracing::Imager {
// Describe how a surface point interacts with light.
class Optics {
public:
	explicit Optics(Color &&matte_color = Color("white"),
					Color &&gloss_color = {}, double opacity = 1.0);

	auto operator<=>(const Optics &) const = default;

	void set_matte_color(const Color &matte_color);
	void set_gloss_color(const Color &gloss_color);
	void set_opacity(double opacity);

	[[nodiscard]] const Color &get_matte_color() const;

	[[nodiscard]] const Color &get_gloss_color() const;

	[[nodiscard]] double get_opacity() const;

private:
	Color matte_color_; // color, intensity of scattered reflection
	Color gloss_color_; // color, intensity of mirror reflection
	double opacity_;    // fraction 0..1 of reflected light
};

/**
 * Scale the glossy and matte parts of reflected light so that an object
 * does not reflect more light than hits it.
 *
 * \param gloss_factor Balance between matte and gloss, ranges from 0 to 1
 * \param raw_matte_color The intensity of scattered reflection
 * \param raw_gloss_color The intensity of scattered reflection
 */
Optics balance_matte_gloss(double gloss_factor,
						   const Color &raw_matte_color = Color("white"),
						   const Color &raw_gloss_color = Color("white"));

} // namespace raytracing::Imager

template <>
struct fmt::formatter<raytracing::Imager::Optics> {
	constexpr auto parse(fmt::format_parse_context &ctx) { return ctx.begin(); }

	constexpr auto format(const raytracing::Imager::Optics &optics,
						  fmt::format_context &ctx) {
		return fmt::format_to(ctx.out(),
							  "(Optics) = (\n\tmatte = {},\n\tgloss = {}\n)",
							  optics.get_matte_color(),
							  optics.get_gloss_color());
	}
};
#endif /* ifndef OPTICS_HPP */
