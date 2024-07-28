#ifndef TRANSFORMABLE_HPP
#define TRANSFORMABLE_HPP

#include <gsl/gsl-lite.hpp>

namespace raytracing {
class [[nodiscard]] RefractiveIndex {
public:
	RefractiveIndex() = delete;

	explicit constexpr RefractiveIndex(float index) noexcept : index_(index) {
		gsl_Expects(index > 0);
	}

	[[nodiscard]] constexpr float
	operator/(const RefractiveIndex &other) const {
		return index_ / other.index_;
	}

	static const RefractiveIndex VACUUM;
	static const RefractiveIndex AIR;
	static const RefractiveIndex ICE;
	static const RefractiveIndex WATER;
	static const RefractiveIndex GASOLINE;
	static const RefractiveIndex GLASS;
	static const RefractiveIndex SAPPHIRE;
	static const RefractiveIndex DIAMOND;

private:
	float index_;
};

inline constexpr RefractiveIndex RefractiveIndex::VACUUM{1.f};
inline constexpr RefractiveIndex RefractiveIndex::AIR{1.0003f};
inline constexpr RefractiveIndex RefractiveIndex::ICE{1.3100f};
inline constexpr RefractiveIndex RefractiveIndex::WATER{1.3330f};
inline constexpr RefractiveIndex RefractiveIndex::GASOLINE{1.3980f};
inline constexpr RefractiveIndex RefractiveIndex::GLASS{1.5500f};
inline constexpr RefractiveIndex RefractiveIndex::SAPPHIRE{1.7700f};
inline constexpr RefractiveIndex RefractiveIndex::DIAMOND{2.4190f};
} // namespace raytracing
#endif /* ifndef TRANSFORMABLE_HPP */
