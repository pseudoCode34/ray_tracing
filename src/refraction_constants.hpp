#ifndef REFRACTION_CONSTANTS_HPP
#define REFRACTION_CONSTANTS_HPP

#include <cassert>

namespace raytracing {
namespace refraction {
//  Refractive indexes of common substances/media...
inline constexpr double VACUUM   = 1.0000;
inline constexpr double AIR      = 1.0003;
inline constexpr double ICE      = 1.3100;
inline constexpr double WATER    = 1.3330;
inline constexpr double GASOLINE = 1.3980;
inline constexpr double GLASS    = 1.5500;
inline constexpr double SAPPHIRE = 1.7700;
inline constexpr double DIAMOND  = 2.4190;

// Range of allowed refraction values...
inline constexpr double MIN = 1.0000;
inline constexpr double MAX = 9.0000;

inline constexpr void validate_refraction(double refraction) {
	assert("Invalid refractive index."
		   && (refraction >= MIN || refraction <= MAX));
}

} // namespace refraction
} // namespace raytracing
#endif /* ifndef CONSTANTS_HPP */
