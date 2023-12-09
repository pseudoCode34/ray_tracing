#include "algebra.hpp"

#include "common_constants.hpp"

#include <cmath>
#include <complex>
#include <cstdlib>

namespace raytracing::Algebra {
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

bool is_positve(double number) { return number > EPSILON; }

std::expected<Eigen::Vector3d, EquationSystemResultType>
eq_sys_solve(const Eigen::Matrix3d &A, const Eigen::Vector3d &b) { // NOLINT
	Eigen::FullPivLU<Eigen::Matrix3d> lu_decomposition(A);
	Eigen::Vector3d x = lu_decomposition.solve(b);

	// Matrix is singular
	if (lu_decomposition.isInvertible()) return x;
	if (auto estimated = A * x; estimated.isApprox(b))
		return std::unexpected(EquationSystemResultType::INFINITE);
	return std::unexpected(EquationSystemResultType::NO_SOLUTION);
}

std::array<double, 2> solve_quadratic(double a, double b, double c) {
	using boost::math::tools::quadratic_roots;
	auto roots = quadratic_roots(a, b, c);
	return std::to_array({roots.first, roots.second});
}

std::array<double, 3> solve_cubic(double a, double b, double c, double d) {
	using boost::math::tools::cubic_roots;
	return cubic_roots(a, b, c, d);
}
} // namespace raytracing::Algebra
