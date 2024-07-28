#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "primitive.hpp"
#include "refractive_index.hpp"

namespace raytracing {
class [[nodiscard]] Material {
public:
	explicit Material(ScaledColorConstRef ambient, ScaledColorConstRef diffuse,
					  ScaledColorConstRef specular, float opacity = 1.f,
					  float reflection      = 1.f,
					  RefractiveIndex index = RefractiveIndex::GLASS,
					  float shininess       = 16.f);

	void set_matte_color(ScaledColor matte_color);

	void set_opacity(float opacity);

	[[nodiscard("getter")]] ScaledColor ambient() const;

	[[nodiscard("getter")]] ScaledColor diffuse() const;

	[[nodiscard("getter")]] ScaledColor specular() const;

	[[nodiscard("getter")]] float shininess() const;

	RefractiveIndex get_refractive_index() const;

	[[nodiscard("getter")]] float get_opacity() const { return opacity_; }

	// TODO: Docs
	[[nodiscard]] Vector3Df light_contribution(float reflectance) const;

private:
	ScaledColor ambient_; // color, intensity of scattered reflection
	ScaledColor diffuse_;
	ScaledColor specular_;
	float opacity_;       // how much light is absorbed or scattered
	float reflection_;
	RefractiveIndex ior_;
	float shininess_;
};

} // namespace raytracing

#endif /* ifndef MATERIAL_HPP */
