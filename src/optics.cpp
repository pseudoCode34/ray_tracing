#include "optics.hpp"

#include <cassert>

namespace raytracing {
namespace Imager {

Optics::Optics(Color &&matte_color, Color &&gloss_color, double opacity)
	: matte_color_(std::move(matte_color)),
	  gloss_color_(std::move(gloss_color)),
	  opacity_(opacity) {
	matte_color_.validate();
	gloss_color_.validate();
	assert(opacity >= 0.0 && opacity <= 1.0);
}

void Optics::set_matte_color(const Color &matte_color) {
	matte_color.validate();
	matte_color_ = matte_color;
}

void Optics::set_gloss_color(const Color &gloss_color) {
	gloss_color.validate();
	gloss_color_ = gloss_color;
}

void Optics::balance_matte_gloss(double gloss_factor,
								 const Color &raw_matte_color,
								 const Color &raw_gloss_color) {
	// Make sure the raw colors have realistic values. Otherwise very high
	// component values can defeat the purpose of trying to balance reflected
	// light levels to realistic ranges.
	raw_matte_color.validate();
	raw_gloss_color.validate();

	assert(gloss_factor >= 0.0 && gloss_factor <= 1.0);
	set_matte_color((1.0 - gloss_factor) * raw_matte_color);
	set_gloss_color(gloss_factor * raw_gloss_color);
}

void Optics::set_opacity(double opacity) {
	assert(opacity >= 0 && opacity <= 1);
	opacity_ = opacity;
}

const Color &Optics::get_matte_color() const { return matte_color_; }

const Color &Optics::get_gloss_color() const { return gloss_color_; }

double Optics::get_opacity() const { return opacity_; }

} // namespace Imager
} // namespace raytracing
