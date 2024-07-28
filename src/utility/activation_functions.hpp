#ifndef COLOR_HPP
#define COLOR_HPP
#include <algorithm>
#include <cmath>
#include <concepts>

namespace raytracing {
template <std::floating_point T>
[[nodiscard]] constexpr T relu(T x) {
	return std::fmax(x, static_cast<T>(0));
}

template <std::floating_point T>
[[nodiscard]] constexpr T saturate(T x) {
	return std::clamp(x, static_cast<T>(0), static_cast<T>(1));
}
} // namespace raytracing
#endif
