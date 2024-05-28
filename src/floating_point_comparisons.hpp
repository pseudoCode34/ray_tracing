#ifndef FLOATING_POINT_COMPARISON
#define FLOATING_POINT_COMPARISON

#include "constants/common.hpp"

#include <cmath>

namespace raytracing::FPC {
//	 The comparison 0.1 + 0.2 == 0.3 fails because of the precision loss when
//	 representing floating point in IEEE-754 format.
//	 But this rule doesn't apply for zero(?), as 0.0 can be represented
//	 perfectly fine as mantissa 0 and significand 0, without precision loss.
//
//	 Instead of comparing with epsilon, classifying it into FP_ZERO category
//	 should be considered(?)
//	 https://stackoverflow.com/a/70196247/10908235
// return std::fpclassify(number) == FP_ZERO;

/* Using absolute epsilon comparison
 * \brief Returns true if \a number is near zero within an absolute precision \a
 * max_abs_error
 *
 * \param max_abs_error An absolute floating point comparison error, defaults to
 * EPSILON
 *
 * \warning This function is NAN insensitive, aka, not NAN-checking
 */
constexpr bool near_zero(float number, float max_abs_error = EPSILON) {
	return std::fabs(number) <= max_abs_error;
}

/**
 * \brief Returns true if \a number is approximately equal to \a target within a
 * tolerance of \a prec using relative epsilon comparison
 *
 * \math |number - target| <= prec * max(|number|, |target|)
 *
 * \param number A float needs comparing
 * \param target A float being compared to
 * \param prec An acceptable floating point error, defaults to EPSILON = 1e-4
 *
 * It's common to compare 2 IEEE-754 64 bit floating points using an absolute
 * epsilon, aka checking their difference are less than an DBL_EPSILON, even in
 * game development. DBL_EPSILON is a machine epsilon indicate, the difference
 * between 1 and the smallest value greater than 1 that is representable [by a
 * float].
 *
 * The reason why the magic number \a EPSILON here is because it's "kind of
 * small". Although, this method works in many cases, i.e, expected difference
 * is less than 1. It fails if 2 floating points are too large that their
 * difference surpass epsilon, aka the epsilon end up being smaller than the
 * smallest rounding error. Therefore, the epsilon comparison is just doing a
 * more-expensive and less-obvious equality check. Additionally, what if the
 * epsilon becomes too high when the numbers being compared are very small? The
 * comparison would return “true” for numbers that are quite different, which is
 * expected to be "false"(not equal). 2 failed case are mentioned in
 * https://stackoverflow.com/a/4548977/10908235
 *
 * \see Relative Epsilon Comparison and Units in the Last Place from
 * https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
 *
 * \see http://floating-point-gui.de/errors/comparison/ For more floating point
 * equality comparison alternatives
 *
 * \see https://stackoverflow.com/a/77735/10908235 Be extremely cautious when
 * presuming a==b if |a-b|<epsilon
 *
 * \see https://stackoverflow.com/a/13699489/10908235 For more representable
 * numbers less than epsilon
 *
 * \warning This function does not check NAN value
 *
 * \see nan_sensitive_float_near()
 */
constexpr bool near(float number, float target, float prec = EPSILON) {
	using std::fabs, std::fmax;
	float diff          = fabs(number - target);
	float max_rel_error = fmax(fabs(number), fabs(target));

	return diff <= prec * max_rel_error;
}

constexpr bool nan_sensitive_near(float number, float target,
								  float prec = EPSILON) {
	using std::isnan;
	// treating two NaNs as equal
	if (isnan(number) && isnan(target)) return true;

	return near(number, target, prec);
}

/**
 * \brief Determines if the floating point number \a number is greater than zero
 * based on absolute precision \a prec
 *
 * \param number A floating-point not NAN or INF checking yet
 * \param prec An absolute floating point comparison error, defaults to EPSILON
 *
 * \note Treat too small number like 1e-6 as zero, so std::signbit may not help.
 *
 * \relatesalso std::fpclassify, FP_INVALID
 */
constexpr bool is_positive(float number, float prec = EPSILON) {
	return number > prec;
}

constexpr bool greater(float number, float target, float prec = EPSILON) {
	using std::fabs, std::fmax;

	float diff          = number - target;
	float max_rel_error = fmax(fabs(number), fabs(target));

	return diff > prec * max_rel_error;
}

} // namespace raytracing::FPC
#endif /* ifndef FLOATING_POINT_COMPARISON */
