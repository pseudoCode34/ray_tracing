#include "algebra.hpp"
#include "algebra_test.hpp"

#include <array>
#include <gtest/gtest.h>
#include <limits>

namespace raytracing::Algebra {
TEST(CubicEquation, RealCoeffComplexRootTreatedAsNan) {
	// given
	using namespace std::complex_literals;
	std::array<double, 3> expected_roots{
		3,
		std::numeric_limits<double>::quiet_NaN(),
		std::numeric_limits<double>::quiet_NaN()};

	// when
	auto actual_roots = solve_cubic(1.0, -3.0, 1.0, -3.0);

	// then
	ASSERT_THAT(actual_roots,
				NanSensitiveElementsNearArray(expected_roots, EPSILON));
}

TEST(CubicEquation, RealCoeffRealRoots) {
	// given
	std::array<double, 3> expected_roots{-1, 0, 1};

	// when
	using boost::math::tools::cubic_roots;
	auto actual_roots = cubic_roots(1., 0., -1., 0.);

	// then
	EXPECT_THAT(actual_roots, ElementsAreArray(expected_roots));
}

} // namespace raytracing::Algebra
