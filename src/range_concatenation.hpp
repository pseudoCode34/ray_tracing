#include <exception>
#include <fmt/ranges.h>
#include <list>
#include <ranges>
#include <span>
#include <tuple>
#include <utility>
#include <vector>

// https://codereview.stackexchange.com/q/284257
namespace detail {

// For convenience, create a type-function that returns the first type of a
// parameter pack.
template <typename T, typename... Rest>
struct GetFirstTypeT {
	using Type = T;
};

template <typename... Pack>
using GetFirstType = GetFirstTypeT<Pack...>::Type;

// The `ExpandedIter` type will be recursively instantiated to implement the
// `ConcatIter` for a concatenated view.
template <typename... Ranges>
struct ExpandedIter;

template <typename R, typename... Ranges>
struct ExpandedIter<R, Ranges...> {
	using EltT  = std::ranges::range_value_t<R>;
	using IterT = std::ranges::iterator_t<R>;
	R &r;
	IterT iter;
	ExpandedIter<Ranges...> rest_iter;

	ExpandedIter(R &r, Ranges &...rest) : r(r), rest_iter(rest...) {
		iter = r.begin();
	}

	constexpr bool is_end() const {
		if (iter != r.end()) return false;
		return rest_iter.is_end();
	}

	constexpr EltT &current() {
		if constexpr (sizeof...(Ranges) != 0) {
			if (iter == r.end()) return rest_iter.current();
		}
		return *iter;
	}

	constexpr void next() {
		if (iter == r.end()) rest_iter.next();
		else ++iter;
	}
};

template <>
struct ExpandedIter<> {
	constexpr bool is_end() const { return true; }

	constexpr void next() {}
};

template <typename... Views>
class ConcatIter {
	ExpandedIter<Views...> exp_iter_;

public:
	using EltT = std::ranges::range_value_t<GetFirstType<Views...>>;

	ConcatIter(Views &...views) : exp_iter_(views...) {}

	ConcatIter(const ConcatIter &)            = delete;
	ConcatIter(ConcatIter &&other)            = default;
	ConcatIter &operator=(const ConcatIter &) = delete;
	ConcatIter &operator=(ConcatIter &&)      = delete;

	constexpr bool is_end() const { return exp_iter_.is_end(); }

	constexpr const EltT &operator*() const { return exp_iter_.current(); }

	constexpr EltT &operator*() { return exp_iter_.current(); }

	constexpr ConcatIter &operator++() {
		exp_iter_.next();
		return *this;
	}
};

struct EndIter {};

template <typename... Rs>
constexpr bool operator==(const ConcatIter<Rs...> &iter, EndIter) {
	return iter.is_end();
}

template <typename... Ranges>
class ConcatView {
	using iterator = ConcatIter<decltype(std::views::all(
		std::forward<Ranges>(std::declval<Ranges &>())))...>;
	std::tuple<decltype(std::views::all(
		std::forward<Ranges>(std::declval<Ranges &>())))...>
		views_;

public:
	ConcatView(Ranges &&...ranges)
		: views_(std::views::all(std::forward<Ranges>(ranges))...) {}

	constexpr iterator begin() {
		return std::make_from_tuple<iterator>(views_);
	}

	constexpr EndIter end() { return {}; }
};

// Concatenate ranges with same (or compatible?) element type.
template <typename... Rs>
	requires (std::ranges::range<Rs> && ...)
constexpr ConcatView<Rs...> concat_helper(Rs &&...rs) {
	return {std::forward<Rs>(rs)...};
}

} // namespace detail

// Concatenate ranges with same (or compatible?) element type.
template <typename... Rs>
	requires (std::ranges::range<Rs> && ...)
constexpr auto concat(Rs &&...rs) {
	return detail::concat_helper(std::forward<Rs>(rs)...);
}
