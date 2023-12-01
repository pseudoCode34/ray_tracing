#include "algebra.hpp"

#include <gtest/gtest.h>

namespace raytracing {
namespace Algebra {

TEST(Matrix3x3DoubleTest, UniqueSolution) {
	// given
	Eigen::Matrix3d A{{1, 6, 2}, {1, 3, 1}, {1, 2, 3}}; // NOLINT
	Eigen::Vector3d b{1, 2, 3};

	// when
	auto solution = eq_sys_solve(A, b);

	// then
	ASSERT_TRUE(solution.has_value());
}

TEST(Matrix3x3DoubleTest, NoSolution) {
	// given
	Eigen::Matrix3d A{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}; // NOLINT
	Eigen::Vector3d b{3, 3, 4};

	// when
	auto solution = eq_sys_solve(A, b);

	// then
	ASSERT_FALSE(solution.has_value());
	EXPECT_EQ(solution.error(), EquationSystemResultType::NO_SOLUTION);
}

TEST(Matrix3x3DoubleTest, InfiniteSolution) {
	// given
	Eigen::Matrix3d A{{1, 2, 3}, {2, 4, 6}, {3, 6, 9}}; // NOLINT
	Eigen::Vector3d b{1, 2, 3};

	// when
	auto solution = eq_sys_solve(A, b);

	// then
	ASSERT_FALSE(solution.has_value());
	EXPECT_EQ(solution.error(), EquationSystemResultType::INFINITE);
}
} // namespace Algebra
} // namespace raytracing
