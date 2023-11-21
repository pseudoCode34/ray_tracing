#ifndef __DDC_ALGEBRA_H
#define __DDC_ALGEBRA_H

#include <complex>
#include <limits>
#include <numbers>
#include <optional>
#include <span>
#include <stdexcept>
#include <string>
#include <vector>

namespace raytracing {
namespace Algebra {
using complex = std::complex<double>;

struct SolverException : std::runtime_error {
	explicit SolverException(const std::string &error);
};

struct LinearRealCoefficients {
	double a, b, c, d;
};

struct LinearEquationSystemRoot {
	double first_root, second_root, third_root;
};

//
//   Solves the linear system of equations for the variables u,v,w:
//
//       Du + Ev + Fw + G = 0
//       Hu + Iv + Jw + K = 0
//       Lu + Mv + Nw + P = 0
//
//   Where D..P are known real numbers.
//
//   If a solution is possible, returns true and sets u, v, w.
//   If no solution exists, returns false and leaves u, v, w unmodified.
//
std::optional<LinearEquationSystemRoot>
solve_linear_equations(std::span<LinearRealCoefficients> coefficent_list);

// Returns n=0..numComplexValues, and fills in outArray
// with the n values from inArray that are real-valued
// (i.e., whose imaginary parts are within TOLERANCE of 0.)
// outArray must be large enough to receive numComplexValues values.
std::vector<double> filter_pure_real_numbers(std::span<complex> in_array);

struct QuadraticComplexCoeffs {
	complex x_2, x_1, x_0;
};

struct QuadraticRealCoeffs {
	double x_2, x_1, x_0;
	QuadraticComplexCoeffs to_complex() const;
};

// Returns n=0..2, the number of distinct complex roots
// found for the equation
//
//     ax^2 + bx + c = 0
//
// Stores the roots in the first n slots of the array 'roots'.
std::vector<complex>
solve_quadratic_equation(const QuadraticComplexCoeffs &coeff);
std::vector<double> solve_quadratic_equation(const QuadraticRealCoeffs &coeff);

struct QuarticComplexCoeffs {
	complex x_4, x_3, x_2, x_1, x_0;

	QuarticComplexCoeffs &operator/=(const complex &other);

	friend QuarticComplexCoeffs operator/(QuarticComplexCoeffs lhs,
										  const complex &rhs);
};

// Represents the coefficients of term with respect to the decreasing degree
struct QuarticRealCoeffs {
	double x_4, x_3, x_2, x_1, x_0;
	QuarticComplexCoeffs to_complex() const;
};

/* struct QuarticComplexCoeffs : std::array<complex, 5> {}; */

/* struct QuarticRealCoeffs : std::array<double, 5> {}; */

// Returns n=0..4, the number of distinct complex roots
// found for the equation
//
//     ax^4 + bx^3 + cx^2 + dx + e = 0
//
// Stores the roots in the first n slots of the array 'roots'.
std::vector<complex> solve_quartic_equation(const QuarticComplexCoeffs &coeff);
std::vector<double> solve_quartic_equation(const QuarticRealCoeffs &coeff);

// The above solvers are generalized for complex-valued coefficients and roots.
// Below are helper methods that accept real-valued
// coefficients and return the subset of roots that are real-valued.

struct CubicComplexCoefficients;
struct CubicRealCoefficients;

struct CubicRealCoefficients {
	double x_3, x_2, x_1, x_0;

	CubicComplexCoefficients to_complex() const;
};

struct CubicComplexCoefficients {
	complex x_3, x_2, x_1, x_0;

	CubicRealCoefficients to_real() const;
};

// Returns n=0..3, the number of distinct real roots found for the equation
//
//     ax^3 + bx^2 + cx + d = 0
//
// Stores the roots in the first n slots of the array 'roots'.
std::vector<complex> solve_cubic_equation(CubicComplexCoefficients coeff);
std::vector<double> solve_cubic_equation(const CubicRealCoefficients &coeff);

bool is_complex_zero(const complex &x);
bool is_fp_zero(double number);
bool is_equal(double lhs, double rhs);
} // namespace Algebra

} // namespace raytracing
#endif // __DDC_ALGEBRA_H
