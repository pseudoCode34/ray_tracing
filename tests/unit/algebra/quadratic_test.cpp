#include "algebra.hpp"
#include "algebra_test.hpp"
#include "constants/common.hpp"

#include "gmock/gmock.h"
#include <complex>
#include <gtest/gtest.h>
#include <utility>

namespace raytracing::Algebra {
TEST(QuadraticEquation, RealCoeffRealRoots) {
	// given
	const Eigen::Vector2f expected_roots{1, 2};

	// when
	const auto actual_roots = minPostiveRootOf(1, -3, 2);

	// then
	using FPC::NanSensitiveElementsNearArray;
	ASSERT_THAT(actual_roots,
				NanSensitiveElementsNearArray(expected_roots, EPSILON));
}

TEST(QuadraticEquation, RealCoeffComplexRoots_Disabled) {
	// given
	using namespace std::complex_literals;
	const auto expected_roots
		= std::make_pair(-1.f - 1.41421356if, -1.f + 1.41421356if);

	// when
	using boost::math::tools::quadratic_roots;
	const auto actual_roots = quadratic_roots(std::complex<float>(1),
											  std::complex<float>(2),
											  std::complex<float>(3));

	// then
	using FPC::ComplexNear;
	using testing::FieldsAre;
	EXPECT_THAT(actual_roots,
				FieldsAre(ComplexNear(expected_roots.first, EPSILON),
						  ComplexNear(expected_roots.second, EPSILON)));
}

TEST(QuadraticEquation, ComplexCoeffAndRoots_Disabled) {
	// given
	using namespace std::complex_literals;
	const auto expected_roots = std::make_pair(1.45216805f + 2.06735566if,
											   -0.25216805f - 0.46735566if);

	// when
	using boost::math::tools::quadratic_roots;
	const auto actual_roots = quadratic_roots(1.f + 2.if, 2.f - 4.if, 3.f);

	// then
	using FPC::ComplexNear;
	using testing::FieldsAre;
	EXPECT_THAT(actual_roots,
				FieldsAre(ComplexNear(expected_roots.first, EPSILON),
						  ComplexNear(expected_roots.second, EPSILON)));
}

} // namespace raytracing::Algebra
