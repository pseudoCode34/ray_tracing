#ifndef ALGEBRA_TEST
#define ALGEBRA_TEST

#include <complex>
#include <fmt/core.h>
#include <gmock/gmock.h>

template <std::floating_point T>
struct fmt::formatter<std::complex<T>> {
	constexpr auto parse(const format_parse_context &ctx)
		-> format_parse_context::iterator {
		return ctx.begin();
	}

	constexpr auto format(const T &p, format_context &ctx) const
		-> format_context::iterator {
		return fmt::format_to(ctx.out(), "({}, {})", p.real(), p.imag());
	}
};

namespace raytracing::FPC {
MATCHER_P2(ComplexNear, expected, max_abs_error, "") {
	return is_complex_near(arg, expected, max_abs_error);
}

MATCHER_P2(ComplexElementsAreNear, expected, max_abs_error, "") {
	if (arg.size() != expected.size()) {
		*result_listener << fmt::format("Size mismatch: expected {}, actual {}",
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

MATCHER_P2(FLoatElementsAreNear, expected, max_abs_error, "") {
	if (arg.size() != expected.size()) {
		*result_listener << fmt::format("Size mismatch: expected {}, actual ",
										expected.size(),
										arg.size());
		return false;
	}

	bool all_matched = true;
	for (size_t i = 0; i < expected.size(); ++i) {
		if (fp_near(arg[i], expected[i], max_abs_error)) {
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

MATCHER_P2(NanSensitiveElementsNearArray, expected, max_abs_error, "") {
	if (arg.size() != expected.size()) {
		*result_listener << "Size mismatch: expected " << expected.size()
						 << ", actual " << arg.size();
		return false;
	}

	for (long i = 0; i < expected.size(); ++i) {
		if (!nan_sensitive_fp_near(arg[i], expected[i], max_abs_error)) {
			*result_listener << "Element mismatch at index " << i
							 << ": expected " << expected[i] << ", actual "
							 << arg[i];
			return false;
		}
	}

	return true;
}
} // namespace raytracing::FPC

#endif /* ifndef ALGEBRA_TEST */
