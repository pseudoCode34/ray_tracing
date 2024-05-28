#ifndef SMART_POINTER_CONCEPT_HPP
#define SMART_POINTER_CONCEPT_HPP

#include <concepts>
#include <gsl/gsl-lite.hpp>
#include <type_traits>

namespace raytracing {
template <typename T>
concept nullptr_default_construction
	= std::default_initializable<T> && requires(T a) {
		  { a == nullptr } -> std::convertible_to<bool>;
	  };


template <typename T>
concept is_smart_pointer = requires(const T &t) {
	{
		t.operator*()
	} -> std::same_as<std::add_lvalue_reference_t<typename T::element_type>>;
	{
		t.operator->()
	} -> std::same_as<std::add_pointer_t<typename T::element_type>>;
} && std::equality_comparable<T> && nullptr_default_construction<T>;

template <typename T>
concept is_unique_ptr
	= is_smart_pointer<T> && std::movable<T> && !std::copyable<T>;

template <typename T>
concept is_shared_ptr = is_smart_pointer<T> && std::copyable<T>;

} // namespace raytracing
#endif /* ifndef SMART_POINTER_CONCEPT_HPP */
