#include "algebra_test.hpp"

#include <boost/math/tools/roots.hpp>
#include <complex>
#include <gtest/gtest.h>
#include <utility>

namespace raytracing {

namespace Algebra {
TEST(QuadraticEquation, RealCoeffRealRoots) {
	// given
	const auto EXPECTED_ROOTS = std::make_pair(1, 2);

	// when
	const auto ACTUAL_ROOTS = boost::math::tools::quadratic_roots(1, -3, 2);

	// then
	EXPECT_THAT(ACTUAL_ROOTS,
				FieldsAre(DoubleNear(EXPECTED_ROOTS.first, EPSILON),
						  DoubleNear(EXPECTED_ROOTS.second, EPSILON)));
}

TEST(QuadraticEquation, RealCoeffComplexRoots) {
	// given
	using namespace std::complex_literals;
	const auto EXPECTED_ROOTS
		= std::make_pair(-1. - 1.41421356i, -1. + 1.41421356i);

	// when
	const auto ACTUAL_ROOTS
		= boost::math::tools::quadratic_roots(std::complex<double>(1),
											  std::complex<double>(2),
											  std::complex<double>(3));

	// then
	EXPECT_THAT(ACTUAL_ROOTS,
				FieldsAre(ComplexNear(EXPECTED_ROOTS.first, EPSILON),
						  ComplexNear(EXPECTED_ROOTS.second, EPSILON)));
}

TEST(QuadraticEquation, ComplexCoeffAndRoots) {
	// given
	using namespace std::complex_literals;
	const auto EXPECTED_ROOTS
		= std::make_pair(1.45216805 + 2.06735566i, -0.25216805 - 0.46735566i);

	// when
	const auto ACTUAL_ROOTS
		= boost::math::tools::quadratic_roots(1. + 2.i, 2. - 4.i, 3. + 0.i);

	// then
	EXPECT_THAT(ACTUAL_ROOTS,
				FieldsAre(ComplexNear(EXPECTED_ROOTS.first, EPSILON),
						  ComplexNear(EXPECTED_ROOTS.second, EPSILON)));
}

} // namespace Algebra
} // namespace raytracing
