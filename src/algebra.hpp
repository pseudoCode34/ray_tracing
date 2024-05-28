#ifndef __DDC_ALGEBRA_H
#define __DDC_ALGEBRA_H

#include <array>
#include <boost/math/tools/cubic_roots.hpp>
#include <Eigen/LU>
#include <expected>

namespace raytracing {
enum class EquationSystemResultType { NO_SOLUTION, INFINITE };

inline std::expected<Eigen::Vector3f, EquationSystemResultType>
eq_sys_solve(const Eigen::Matrix3f &A, const Eigen::Vector3f &b) { // NOLINT
	Eigen::FullPivLU<Eigen::Matrix3f> lu_decomposition(A);
	Eigen::Vector3f x = lu_decomposition.solve(b);

	// Matrix is singular
	if (lu_decomposition.isInvertible()) return x;
	if (auto estimated = A * x; estimated.isApprox(b))
		return std::unexpected(EquationSystemResultType::INFINITE);
	return std::unexpected(EquationSystemResultType::NO_SOLUTION);
}

constexpr std::array<float, 3> solve_cubic(float a, float b, float c, float d) {
	using boost::math::tools::cubic_roots;
	return cubic_roots(a, b, c, d);
}
} // namespace raytracing

#endif // __DDC_ALGEBRA_H
