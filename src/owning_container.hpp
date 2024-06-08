#ifndef OWNING_COLLECTION_HPP
#define OWNING_COLLECTION_HPP

#include "owning_container/concepts.hpp"
#include "owning_container/concepts/smart_pointer_concept.hpp"

#include <algorithm>
#include <memory>
#include <optional>
#include <vector>

namespace raytracing {
template <class T, template <typename...> class Owner = std::unique_ptr,
		  template <typename...> class Container = std::vector>
class OwningContainer {
public:
	using element_type = T;
	using pointer      = Owner<element_type>;
	using value_type   = Container<pointer>;

	static_assert(is_smart_pointer<pointer>);
	static_assert(is_stl_container<value_type>);

	void insert(pointer &&x) { container_.push_back(std::move(x)); }

	void insert(element_type x) {
		if constexpr (is_unique_ptr<pointer>)
			container_.push_back(std::make_unique<element_type>(x));
		else container_.push_back(std::make_shared<element_type>(x));
	}

	constexpr void clear() { container_.clear(); }

	void swap(OwningContainer &other) noexcept { container_.swap(other); }

	const element_type &operator[](size_t i) const {
		return *container_[i].get();
	}

	constexpr auto transform(std::invocable<element_type> auto &&op) const {
		return container_
			   | std::ranges::transform(std::forward<decltype(op)>(op));
	}

	constexpr bool any_match(std::predicate<pointer> auto &&pred) const {
		return std::ranges::any_of(container_,
								   std::forward<decltype(pred)>(pred));
	}

	constexpr std::optional<element_type *>
	find_any(std::predicate<pointer> auto &&pred) const {
		const auto result
			= std::ranges::find_if(container_,
								   std::forward<decltype(pred)>(pred));
		if (result != std::ranges::end(container_)) return std::nullopt;
		return (*result).get();
	}

	const value_type &container() const { return container_; }

private:
	value_type container_;
};
} // namespace raytracing
#endif /* ifndef OWNING_COLLECTION_HPP */
