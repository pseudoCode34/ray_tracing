#include "algebra.hpp"

#include "common_constants.hpp"

#include <array>
#include <cassert>
#include <cmath>
#include <complex>
#include <cstdlib>
#include <optional>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/transform.hpp>
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
bool is_fp_zero(double number) {
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

bool is_equal(double lhs, double rhs) { return is_fp_zero(lhs - rhs); }

bool is_complex_zero(const complex &x) {
	return abs(x.real()) < EPSILON && abs(x.imag()) < EPSILON;
}

SolverException::SolverException(const std::string &error)
	: std::runtime_error{error} {}

std::optional<LinearEquationSystemRoot>
solve_linear_equations(std::span<LinearRealCoefficients> coefficent_list) {
	// The intermediate variable names reflect how I solved the linear equations
	// by hand with pencil and paper. But we actually calculate their values in
	// somewhat non-alphabetical order because that minimizes the CPU time in
	// case we have to return false. In other words, we do the least amount of
	// work needed to figure out whether there might not be a solution, before
	// bothering to run all the calculations. Also, we try to help
	// branch-prediction of most compilers by assuming that most of the time the
	// solution does exist: the 'if' conditions are assumed to be true more
	// often than false, minimizing branch prediction failures.

	const auto &[a1, b1, c1, d1] = coefficent_list[0];
	const auto &[a2, b2, c2, d2] = coefficent_list[1];
	const auto &[a3, b3, c3, d3] = coefficent_list[2];

	if (is_fp_zero(c1)) return {};
	if (double new_b = b1 * c2 - c1 * b2; !is_fp_zero(new_b)) {
		const double NEW_A = a1 * c2 - c1 * a2;
		const double NEW_D = a2 * c3 - c2 * a3;
		const double NEW_E = b2 * c3 - c2 * b3;
		const double DENOM = NEW_A * NEW_E - new_b * NEW_D;
		if (is_fp_zero(DENOM)) return {};

		const double NEW_C = d1 * c2 - c1 * d2;
		const double NEW_F = d2 * c3 - c2 * d3;

		double first_root  = (new_b * NEW_F - NEW_E * NEW_C) / DENOM,
			   second_root = -(NEW_A * first_root + NEW_C) / new_b,
			   third_root  = -(a1 * first_root + b1 * second_root + d1) / c1;

		return LinearEquationSystemRoot{first_root, second_root, third_root};
	}
	return {};
}

std::vector<double> filter_pure_real_numbers(std::span<complex> in_array) {
	/* int num_real_values = 0; */
	/* for (int i = 0; i < num_complex_values; ++i) */
	/* 	if (!is_close_to_zero(in_array[i].imag())) */
	/* 		out_array[num_real_values++] = in_array[i].real(); */
	/* return num_real_values; */
	return in_array | ranges::views::filter([](const auto &p) {
			   return is_fp_zero(p.imag());
		   })
		   | ranges::views::transform([](const auto &p) { return p.real(); })
		   | ranges::to<std::vector<double>>();
}

// Returns n=0..2, the number of distinct real roots found for the equation
//
//     ax^2 + bx + c = 0
//
// Stores the roots in the first n slots of the array 'roots'.
std::vector<complex>
solve_quadratic_equation(const QuadraticComplexCoeffs &coeff) {
	const auto &[a, b, c] = coeff;
	if (is_complex_zero(a)) {
		if (is_complex_zero(b)) {
			// The equation devolves to: c = 0, where the variable x has
			// vanished!
			return {};   // cannot divide by zero, so there is no solution.
		}                // Simple linear equation: bx + c = 0, so x = -c/b.
		return {-c / b}; // there is a single solution.
	}

	const complex RADICAND = b * b - 4.0 * a * c;
	if (is_complex_zero(RADICAND)) {
		// Both roots have the same value: -b / 2a.
		return {-b / (a * 2.0)};
	}

	// There are two distinct real roots.
	const complex R = sqrt(RADICAND);
	const complex D = a * 2.0;

	return {(-b + R) / D, (-b - R) / D};
}

complex cbrt(complex a, int n) {
	const double TWO_PI = pi * 2;

	double rho   = pow(abs(a), 1.0 / 3.0);
	double theta = (TWO_PI * n + arg(a)) / 3.0;
	return complex(rho * cos(theta), rho * sin(theta));
}

std::vector<complex> solve_cubic_equation(CubicComplexCoefficients coeff) {
	auto &&[a, b, c, d] = coeff;
	if (is_complex_zero(a))
		return solve_quadratic_equation(QuadraticComplexCoeffs{b, c, d});

	b /= a;
	c /= a;
	d /= a;

	complex S     = b / 3.0;
	complex D     = c / 3.0 - S * S;
	complex E     = S * S * S + (d - S * c) / 2.0;
	complex Froot = sqrt(E * E + D * D * D);
	complex F     = -Froot - E;

	if (is_complex_zero(F)) F = Froot - E;

	std::vector<complex> roots;
	for (int i = 0; i < 3; ++i) {
		const complex G = cbrt(F, i);
		roots.push_back(G - D / G - S);
	}

	return roots;
}

// Returns n=0..4, the number of distinct real roots found for the equation
//
//     ax^4 + bx^3 + cx^2 + dx + e = 0
//
// Stores the roots in the first n slots of the array 'roots'.
std::vector<complex> solve_quartic_equation(const QuarticComplexCoeffs &coeff) {
	/* auto &&[a, b, c, d, e] = coeff.data(); */
	auto &&[a, b, c, d, e] = coeff;
	if (is_complex_zero(a))
		return solve_cubic_equation(CubicComplexCoefficients{b, c, d, e});
	// See "Summary of Ferrari's Method" in
	// http://en.wikipedia.org/wiki/Quartic_function

	// Without loss of generality, we can divide through by 'a'.
	// Anywhere 'a' appears in the equations, we can assume a = 1.
	QuarticComplexCoeffs scaled_coeffs = coeff / a;

	auto &&[a_scaled, b_scaled, c_scaled, d_scaled, e_scaled] = coeff;
	complex b2 = b_scaled * b_scaled;
	complex b3 = b_scaled * b2;
	complex b4 = b2 * b2;

	complex alpha = (-3.0 / 8.0) * b2 + c_scaled;
	complex beta  = b3 / 8.0 - b_scaled * c_scaled / 2.0 + d_scaled;
	complex gamma = (-3.0 / 256.0) * b4 + b2 * c_scaled / 16.0
					- b_scaled * d_scaled / 4.0 + e_scaled;

	complex alpha2 = alpha * alpha;
	complex t      = -b_scaled / 4.0;

	if (is_complex_zero(beta)) {
		complex rad = sqrt(alpha2 - 4.0 * gamma);
		complex r1  = sqrt((-alpha + rad) / 2.0);
		complex r2  = sqrt((-alpha - rad) / 2.0);

		return {t + r1, t - r1, t + r2, t - r2};
	}
	complex alpha3 = alpha * alpha2;
	complex p      = -(alpha2 / 12.0 + gamma);
	complex q      = -alpha3 / 108.0 + alpha * gamma / 3.0 - beta * beta / 8.0;
	complex r      = -q / 2.0 + sqrt(q * q / 4.0 + p * p * p / 27.0);
	complex u      = cbrt(r, 0);
	complex y      = (-5.0 / 6.0) * alpha + u;
	if (is_complex_zero(u)) y -= cbrt(q, 0);
	else y -= p / (3.0 * u);
	complex w = sqrt(alpha + 2.0 * y);

	complex r1 = sqrt(-(3.0 * alpha + 2.0 * y + 2.0 * beta / w));
	complex r2 = sqrt(-(3.0 * alpha + 2.0 * y - 2.0 * beta / w));

	return {t + (w - r1) / 2.0,
			t + (w + r1) / 2.0,
			t + (-w - r2) / 2.0,
			t + (-w + r2) / 2.0};
}

// FIXME: copy semantics?
std::vector<double> solve_cubic_equation(const CubicRealCoefficients &coeff) {
	std::vector<complex> croots = solve_cubic_equation(coeff.to_complex());

	return filter_pure_real_numbers(croots);
}

std::vector<double> solve_quartic_equation(const QuarticRealCoeffs &coeff) {
	std::vector<complex> croots = solve_quartic_equation(coeff.to_complex());

	return filter_pure_real_numbers(croots);
}

std::vector<double> solve_quadratic_equation(const QuadraticRealCoeffs &coeff) {
	const auto [a, b, c] = coeff;
	if (is_fp_zero(a)) {
		if (is_fp_zero(b)) {
			// The equation devolves to: c = 0, where the variable x has
			// vanished!
			return {};   // cannot divide by zero, so there is no solution.
		}                // Simple linear equation: bx + c = 0, so x = -c/b.
		return {-c / b}; // there is a single solution.
	}

	const double RADICAND = b * b - 4.0 * a * c;
	if (is_fp_zero(RADICAND)) {
		// Both roots have the same value: -b / 2a.
		return {-b / (a * 2.0)};
	}
	if (RADICAND < 0) return {};
	// There are two distinct real roots.
	const double DELTA_SQRT = sqrt(RADICAND), DENOM = a * 2.0;

	return {(-b + DELTA_SQRT) / DENOM, (-b - DELTA_SQRT) / DENOM};
}

CubicRealCoefficients CubicComplexCoefficients::to_real() const {
	return {x_3.real(), x_2.real(), x_1.real(), x_0.real()};
}

CubicComplexCoefficients CubicRealCoefficients::to_complex() const {
	return {x_3, x_2, x_1, x_0};
}

QuadraticComplexCoeffs QuadraticRealCoeffs::to_complex() const {
	return {x_2, x_1, x_0};
}

QuarticComplexCoeffs QuarticRealCoeffs::to_complex() const {
	return {x_4, x_3, x_2, x_1, x_0};
}

QuarticComplexCoeffs &QuarticComplexCoeffs::operator/=(const complex &other) {
	x_4 /= other;
	x_3 /= other;
	x_2 /= other;
	x_1 /= other;
	x_0 /= other;
	return *this;
}

QuarticComplexCoeffs operator/(QuarticComplexCoeffs lhs, const complex &rhs) {
	return lhs /= rhs;
}
} // namespace Algebra
} // namespace raytracing
