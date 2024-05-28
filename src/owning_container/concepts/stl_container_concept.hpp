#ifndef STL_CONTAINER_CONCEPT_HPP
#define STL_CONTAINER_CONCEPT_HPP

#include <string>

namespace raytracing {
template <class T>
concept default_erasable = requires(T *p) { std::destroy_at(p); };

template <class T, class Trait, class Alloc>
concept allocator_erasable = requires(Alloc m, T *p) {
	requires std::same_as<
		typename Trait::allocator_type,
		typename std::allocator_traits<Alloc>::template rebind_alloc<T>>;
	std::allocator_traits<Alloc>::destroy(m, p);
};

template <class T>
concept allocator_aware = requires(T a) {
	{ a.get_allocator() } -> std::same_as<typename T::allocator_type>;
};

template <class T>
concept std_string = std::same_as<std::string, T>;

template <class E, class T>
concept erasable = (std_string<T> && default_erasable<E>)
				   || (allocator_aware<T>
					   && allocator_erasable<E, T, typename T::allocator_type>)
				   || (!allocator_aware<T> && default_erasable<E>);

template <class T>
concept is_stl_container = requires(T lhs, const T RHS) {
	requires std::regular<T>;
	requires std::swappable<T>;
	requires erasable<typename T::value_type, T>;
	requires std::same_as<typename T::reference, typename T::value_type &>;
	requires std::same_as<typename T::const_reference,
						  const typename T::value_type &>;
	requires std::forward_iterator<typename T::iterator>;
	requires std::forward_iterator<typename T::const_iterator>;
	requires std::signed_integral<typename T::difference_type>;
	requires std::same_as<
		typename T::difference_type,
		typename std::iterator_traits<typename T::iterator>::difference_type>;
	requires std::same_as<typename T::difference_type,
						  typename std::iterator_traits<
							  typename T::const_iterator>::difference_type>;
	{ lhs.begin() } -> std::same_as<typename T::iterator>;
	{ lhs.end() } -> std::same_as<typename T::iterator>;
	{ RHS.begin() } -> std::same_as<typename T::const_iterator>;
	{ RHS.end() } -> std::same_as<typename T::const_iterator>;
	{ lhs.cbegin() } -> std::same_as<typename T::const_iterator>;
	{ lhs.cend() } -> std::same_as<typename T::const_iterator>;
	{ lhs.size() } -> std::same_as<typename T::size_type>;
	{ lhs.max_size() } -> std::same_as<typename T::size_type>;
	{ lhs.empty() } -> std::convertible_to<bool>;
};
} // namespace raytracing
#endif /* ifndef STL_CONTAINER_CONCEPT_HPP */
