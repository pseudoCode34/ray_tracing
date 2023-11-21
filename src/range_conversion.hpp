#ifndef RANGE_CONVERSION_HPP
#define RANGE_CONVERSION_HPP

#include <ranges>
#include <vector>
#if !defined(__cpp_lib_ranges_to_container) || __cplusplus < 202202L
// https://stackoverflow.com/a/60971856/10908235

// First go, just as a function taking a range and converting
// to a vector only
template <std::ranges::range R>
constexpr auto to_vector(R &&r) {
	using elem_t = std::decay_t<std::ranges::range_value_t<R>>;
	return std::vector<elem_t>{r.begin(), r.end()};
}

// Second go, using ranges style piping
namespace detail {
// Type acts as a tag to find the correct operator| overload
template <typename C>
struct to_helper {};

// This actually does the work
template <typename Container, std::ranges::range R>
	requires std::convertible_to<std::ranges::range_value_t<R>,
								 typename Container::value_type>
// FIXME: Constexpr this?
constexpr Container operator|(R &&r, to_helper<Container>) {
	return {r.begin(), r.end()};
}
} // namespace detail

// Couldn't find an concept for container, however a
// container is a range, but not a view.
template <std::ranges::range Container>
	requires (!std::ranges::view<Container>)
// FIXME: Mark this function constexpr, aka, inline?
constexpr auto to() {
	return detail::to_helper<Container>{};
}
#endif

#endif /* ifndef RANGE_CONVERSION_HPP */
