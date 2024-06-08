#ifndef UTILITY_HPP
#define UTILITY_HPP
#include <algorithm>
#include <cmath>
#include <concepts>

namespace raytracing {
constexpr float relu(float x) { return std::fmax(x, 0.f); }

constexpr float saturate(float x) { return std::clamp(x, 0.f, 1.f); }

/**
 * Clang17 and below hasn't supported C++20 Class types and floating-point types
 * in non-type template parameters feature yet.
 * Clang18 fixed it.
 * https://github.com/llvm/llvm-project/issues/70214
 */
#if defined(__cpp_nontype_template_args)                                       \
	&& (__cpp_nontype_template_args == 201911L)

template <std::floating_point T, float Low = 0.f, float High = 1.f>
constexpr bool is_in_range(T x) {
	return Low <= x && x <= High;
}
#else
template <std::floating_point T>
constexpr bool is_in_range(T x, T low = static_cast<T>(0),
						   T high = static_cast<T>(1)) {
	return low <= x && x <= high;
}
#endif
} // namespace raytracing
#endif /* ifndef UTILITY_HPP */
