#include "material.hpp"

#include <gsl/gsl-lite.hpp>
#include <utility>

#include "spatial/primitive.hpp"
#include "spatial/refractive_index.hpp"

namespace raytracing {
Material::Material(ScaledColorConstRef ambient, ScaledColorConstRef diffuse,
				   ScaledColorConstRef specular, float opacity,
				   float reflection, RefractiveIndex index, float shininess)
	: ambient_(ambient),
	  diffuse_(diffuse),
	  specular_(specular),
	  opacity_(opacity),
	  reflection_(reflection),
	  ior_(index),
	  shininess_(shininess) {
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

ScaledColor Material::ambient() const { return ambient_; }

ScaledColor Material::diffuse() const { return diffuse_; }

ScaledColor Material::specular() const { return specular_; }

RefractiveIndex Material::get_refractive_index() const { return ior_; }

float Material::shininess() const { return shininess_; }

Vector3Df Material::light_contribution(float reflectance) const {
	gsl_Expects(0 <= reflectance && reflectance <= 1);

	float local        = opacity_ * (1 - reflection_);
	float reflect      = opacity_ * reflection_ + (1 - opacity_) * reflectance;
	float transmission = (1 - opacity_) * (1 - reflectance);
	return {local, reflect, transmission};
}
} // namespace raytracing
