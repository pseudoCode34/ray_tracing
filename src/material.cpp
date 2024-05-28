#include "material.hpp"

#include <gsl/gsl-lite.hpp>
#include <utility>

namespace raytracing {
Material::Material(ScaledColor matte_color, float opacity, float reflection,
				   float refractive_index)
	: ambient_(std::move(matte_color)),
	  opacity_(opacity),
	  reflection_(reflection),
	  refractive_index_(refractive_index) {
	gsl_Expects(opacity >= 0.f && opacity <= 1.f);
	gsl_Expects(0 <= reflection && reflection <= 1);
}

void Material::set_matte_color(ScaledColor matte_color) {
	ambient_ = std::move(matte_color);
}

void Material::set_opacity(float opacity) {
	gsl_Expects(opacity >= 0 && opacity <= 1);
	opacity_ = opacity;
}

ScaledColor Material::attenuation() const { return ambient_; }

float Material::get_refractive_index() const { return refractive_index_; }
} // namespace raytracing
