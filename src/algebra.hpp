#ifndef __DDC_ALGEBRA_H
#define __DDC_ALGEBRA_H
#include "common_constants.hpp"

#include <Eigen/Dense>
#include <complex>
#include <expected>

namespace raytracing {
namespace Algebra {
bool is_near_zero(const std::complex<double> &x);
bool is_near_zero(double number);
bool is_double_near(double lhs, double rhs);

enum class EquationSystemResultType { NO_SOLUTION, INFINITE };

std::expected<Eigen::Vector3d, EquationSystemResultType>eq_sys_solve(
	const Eigen::Matrix3d &a, const Eigen::Vector3d &b);

struct TrigValues {
	double cos_value;
	double sin_value;
};

constexpr TrigValues calculate_cos_sin(double angle_in_degrees) {
	double angle_in_radians = radian_from_degree(angle_in_degrees);

	return {.cos_value = std::cos(angle_in_radians),
			.sin_value = std::sin(angle_in_radians)};
}

} // namespace Algebra
} // namespace raytracing
#endif // __DDC_ALGEBRA_H
