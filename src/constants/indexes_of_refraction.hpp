#ifndef INDEXES_OF_REFRACTION_HPP
#define INDEXES_OF_REFRACTION_HPP

// Index of refraction
namespace raytracing::ior {
inline constexpr float VACUUM   = 1;
inline constexpr float AIR      = 1.0003;
inline constexpr float ICE      = 1.3100;
inline constexpr float WATER    = 1.3330;
inline constexpr float GASOLINE = 1.3980;
inline constexpr float GLASS    = 1.5500;
inline constexpr float SAPPHIRE = 1.7700;
inline constexpr float DIAMOND  = 2.4190;

inline constexpr float MIN = 1.f;
inline constexpr float MAX = 9.f;

} // namespace raytracing::ior

#endif /* ifndef INDEXES_OF_REFRACTION_HPP */
