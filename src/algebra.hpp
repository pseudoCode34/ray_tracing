#ifndef __DDC_ALGEBRA_H
#define __DDC_ALGEBRA_H
#include "common_constants.hpp"

#include <boost/math/tools/quartic_roots.hpp>
#include <complex>
#include <Eigen/Dense>
#include <expected>

namespace raytracing::Algebra {
/*
 * Most people would compare 2 IEEE-754 64 bit floating points using an absolute
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
bool is_near_zero(double number);
bool is_near_zero(const std::complex<double> &x);
bool is_double_near(double lhs, double rhs);
bool is_positve(double number);

enum class EquationSystemResultType { NO_SOLUTION, INFINITE };

std::expected<Eigen::Vector3d, EquationSystemResultType>
eq_sys_solve(const Eigen::Matrix3d &A, const Eigen::Vector3d &b); // NOLINT

struct TrigValues {
	double cos_value;
	double sin_value;
};

constexpr TrigValues calculate_cos_sin(double angle_in_degrees) {
	const double ANGLE_IN_RADIANS = radian_from_degree(angle_in_degrees);

	return {.cos_value = std::cos(ANGLE_IN_RADIANS),
			.sin_value = std::sin(ANGLE_IN_RADIANS)};
}

std::array<double, 2> solve_quadratic(double a, double b, double c);
std::array<double, 3> solve_cubic(double a, double b, double c, double d);
} // namespace raytracing::Algebra

#endif // __DDC_ALGEBRA_H
