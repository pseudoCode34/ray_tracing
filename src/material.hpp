#ifndef OPTICS_HPP
#define OPTICS_HPP

#include "color.hpp"
#include "constants/indexes_of_refraction.hpp"

namespace raytracing {
// Describe how a surface point interacts with light.
class Material {
public:
	Material(ScaledColor ambient = {}, float opacity = 1.f,
			 float reflection = 1.f, float refractive_index = ior::GLASS);

	void set_matte_color(ScaledColor matte_color);
	void set_opacity(float opacity);

	[[nodiscard]] ScaledColor attenuation() const;
	[[nodiscard]] float get_refractive_index() const;

	[[nodiscard]] float get_opacity() const { return opacity_; }

	[[nodiscard]] Vector3f light_contribution(float reflectance) const {
		assert(0 <= reflectance && reflectance <= 1);
		float local   = opacity_ * (1 - reflection_);
		float reflect = opacity_ * reflection_ + (1 - opacity_) * reflectance;
		float transmission = (1 - opacity_) * (1 - reflectance);
		return {local, reflect, transmission};
	}

private:
	ScaledColor ambient_; // color, intensity of scattered reflection
	float opacity_;       // how much light is absorbed or scattered
	float reflection_;
	float refractive_index_;
};
} // namespace raytracing

#endif /* ifndef OPTICS_HPP */
