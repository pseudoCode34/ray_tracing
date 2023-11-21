#ifndef ALGEBRA_TEST
#define ALGEBRA_TEST

#include "common_constants.hpp"

#include <complex>
#include <concepts>
#include <fmt/format.h>
#include <gmock/gmock-matchers.h>
#include <gmock/gmock.h>

template <typename T>
concept ComplexNumber = requires(T a) {
	{ typename T::value_type{} } -> std::same_as<typename T::value_type>;

	{ a.real() } -> std::same_as<typename T::value_type>;
	{ a.imag() } -> std::same_as<typename T::value_type>;
};

template <ComplexNumber T>
struct fmt::formatter<T> {
	constexpr auto parse(format_parse_context &ctx)
		-> format_parse_context::iterator {
		return ctx.begin();
	}

	auto format(const T &p, format_context &ctx) const
		-> format_context::iterator {
		return fmt::format_to(ctx.out(), "({}, {})", p.real(), p.imag());
	}
};

namespace raytracing {
namespace Algebra {

using namespace testing;
auto custom_complex_less
	= []<ComplexNumber T> [[nodiscard]] (const T &lhs, const T &rhs) {
		  return std::norm(lhs) < std::norm(rhs);
	  };

// FIXME: can I templated this function like this
/* template<ComplexNumber T, std::floating_point T> */
/* bool is_complex_near(const T &actual, const T &expected, */
/* 					 Float max_abs_error = EPSILON) { */
inline bool is_complex_near(const std::complex<double> &actual,
							const std::complex<double> &expected,
							double max_abs_error) {
	return std::abs(actual.real() - expected.real()) < max_abs_error
		   && std::abs(actual.imag() - expected.imag()) < max_abs_error;
}

inline bool is_double_near(double actual, double expected,
						   double max_abs_error) {
	return std::abs(std::abs(actual - expected)) <= max_abs_error;
}

template <ComplexNumber T, std::floating_point Float>
bool operator==(const T &lhs, const T &rhs) {
	return std::norm(lhs) < std::norm(rhs);
}

MATCHER_P2(ComplexNear, expected, max_abs_error, "") {
	return is_complex_near(arg, expected, max_abs_error);
}

MATCHER_P2(ComplexElementsAreNear, expected, max_abs_error, "") {
	if (arg.size() != expected.size()) {
		*result_listener << fmt::format("Size mismatch: expected {}, actual ",
										expected.size(),
										arg.size());
		return false;
	}

	bool all_matched = true;
	for (size_t i = 0; i < expected.size(); ++i) {
		if (!is_complex_near(arg[i], expected[i], max_abs_error)) {
			*result_listener << fmt::format(
				"\nElement mismatch at index {} : expected {}, actual {}",
				i,
				expected[i],
				arg[i]);
			all_matched = false;
		}
	}

	return all_matched;
}

MATCHER_P2(DoubleElementsAreNear, expected, max_abs_error, "") {
	if (arg.size() != expected.size()) {
		*result_listener << fmt::format("Size mismatch: expected {}, actual ",
										expected.size(),
										arg.size());
		return false;
	}

	bool all_matched = true;
	for (size_t i = 0; i < expected.size(); ++i) {
		if (is_double_near(arg[i], expected[i], max_abs_error)) {
			*result_listener << fmt::format(
				"\nElement mismatch at index {} : expected {}, actual {}",
				i,
				expected[i],
				arg[i]);
			all_matched = false;
		}
	}

	return all_matched;
}

inline bool nan_sensitive_double_near(double actual, double expected,
									  double max_abs_error) {
	if (std::isnan(actual) && std::isnan(expected)) return true;
	if (is_double_near(actual, expected, max_abs_error)) return true;
	return false;
}

MATCHER_P2(NanSensitiveElementsNearArray, expected, max_abs_error, "") {
	if (arg.size() != expected.size()) {
		*result_listener << "Size mismatch: expected " << expected.size()
						 << ", actual " << arg.size();
		return false;
	}

	for (size_t i = 0; i < expected.size(); ++i) {
		if (!nan_sensitive_double_near(arg[i], expected[i], max_abs_error)) {
			*result_listener << "Element mismatch at index " << i
							 << ": expected " << expected[i] << ", actual "
							 << arg[i];
			return false;
		}
	}

	return true;
}
} // namespace Algebra
} // namespace raytracing
#endif /* ifndef ALGEBRA_TEST */
