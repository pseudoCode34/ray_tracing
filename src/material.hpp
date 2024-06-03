#ifndef OPTICS_HPP
#define OPTICS_HPP

#include "color.hpp"
#include "constants/indexes_of_refraction.hpp"

namespace raytracing {
// Describe how a surface point interacts with light.
class Material {
public:
	explicit Material(ScaledColorConstRef ambient, ScaledColorConstRef diffuse,
					  ScaledColorConstRef specular, float opacity = 1.f,
					  float reflection       = 1.f,
					  float refractive_index = ior::GLASS,
					  float shininess        = 16.f);

	void set_matte_color(ScaledColor matte_color);

	void set_opacity(float opacity);

	[[nodiscard("getter")]] ScaledColor ambient() const;

	[[nodiscard("getter")]] ScaledColor diffuse() const;

	[[nodiscard("getter")]] ScaledColor specular() const;

	[[nodiscard("getter")]] float shininess() const;

	[[nodiscard("getter")]] float get_refractive_index() const;

	[[nodiscard("getter")]] float get_opacity() const { return opacity_; }

	// TODO: Docs
	[[nodiscard]] Vector3f light_contribution(float reflectance) const;

private:
	ScaledColor ambient_; // color, intensity of scattered reflection
	ScaledColor diffuse_;
	ScaledColor specular_;
	float opacity_;       // how much light is absorbed or scattered
	float reflection_;
	float refractive_index_;
	float shininess_;
};

} // namespace raytracing

#endif /* ifndef OPTICS_HPP */
