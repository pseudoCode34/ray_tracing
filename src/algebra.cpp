#include "algebra.hpp"

#include "common_constants.hpp"

#include <cmath>
#include <complex>
#include <cstdlib>
#include <span>
#include <vector>

namespace raytracing {
namespace Algebra {
/* Most people would compare 2 IEEE-754 64 bit floating points using an absolute
 * epsilon, aka checking their difference are less than an DBL_EPSILON, even in
 * game development. DBL_EPSILON is a machine epsilon indicate, the difference
 * between 1 and the smallest value greater than 1 that is representable [by a
 * double].
 *
 * The reason why we choose epsilon as the magic number here is because it
 * "looks small". Although, this method works in many cases, i.e, expected
 * difference is less than 1. It would fail if 2 floating points are too large
 * that their difference surpass epsilon, aka the epsilon end up being
 * smaller than the smallest rounding error. Therefore, the epsilon comparison
 * is just doing a more-expensive and less-obvious equality check. Additionally,
 * what if the epsilon becomes too high when the numbers being compared are very
 * small? The comparison would return “true” for numbers that are quite
 * different, which is expected to be "false"(not equal). 2 failed case are
 * mentioned in https://stackoverflow.com/a/4548977/10908235
 *
 * \see Relative Epsilon Compararison and Units in the Last Place from
 * https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
 *
 * \see http://floating-point-gui.de/errors/comparison/ For more floating point
 * equality comparison alternatives
 *
 * \see https://stackoverflow.com/a/77735/10908235 Be extremely cautious when
 * presuming a==b if |a-b|<epsilon
 *
 * \see https://stackoverflow.com/a/13699489/10908235 For more representible
 * numbers less than epsilon
 *
 */
bool is_near_zero(double number) {
	// The comparison 0.1 + 0.2 == 0.3 fails because of the precision loss when
	// representing floating point in IEEE-754 format.
	// But this rule doesn't apply for zero(?), as 0.0 can be represented
	// perfectly fine as mantissa 0 and significand 0, without precision loss.

	// Instead of comparing with epsilon, classifying it into FP_ZERO category
	// should be considered(?)
	// https://stackoverflow.com/a/70196247/10908235
	/* return std::fpclassify(number) == FP_ZERO; */

	// Using absolute epsilon comparison
	return std::abs(number) <= EPSILON;
}

bool is_double_near(double lhs, double rhs) { return is_near_zero(lhs - rhs); }

bool is_near_zero(const std::complex<double> &x) {
	return abs(x.real()) < EPSILON && abs(x.imag()) < EPSILON;
}

std::expected<Eigen::Vector3d, EquationSystemResultType>
eq_sys_solve(const Eigen::Matrix3d &A, const Eigen::Vector3d &b) { // NOLINT
	Eigen::FullPivLU<Eigen::Matrix3d> lu_decomposition(A);
	Eigen::Vector3d x = lu_decomposition.solve(b);

	// Matrix is singular
	if (lu_decomposition.isInvertible()) return x;
	else if (auto estimated = A * x; estimated.isApprox(b))
		return std::unexpected(EquationSystemResultType::INFINITE);
	else return std::unexpected(EquationSystemResultType::NO_SOLUTION);
}
} // namespace Algebra
} // namespace raytracing
