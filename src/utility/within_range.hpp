#ifndef POINT3D_HPP
#define POINT3D_HPP

#include <algorithm>
#include <concepts>

namespace raytracing {
/**
 * \note Clang17 and below hasn't supported C++20 Class types and floating-point
 * types in non-type template parameters feature yet. Clang18 fixed it.
 * https://github.com/llvm/llvm-project/issues/70214
 */
#if defined(__cpp_nontype_template_args)                                       \
	&& (__cpp_nontype_template_args == 201911L)

template <std::floating_point T, T Low = static_cast<T>(0),
		  T High = static_cast<T>(1)>
[[nodiscard]] constexpr bool within_range(T x) {
	return Low <= x && x <= High;
}
#else
template <std::floating_point T>
[[nodiscard]] constexpr bool within_range(T x, T low = static_cast<T>(0),
										  T high = static_cast<T>(1)) {
	return std::clamp(x, low, high) == x;
}
#endif
} // namespace raytracing
#endif /* ifndef POINT3D_HPP */
