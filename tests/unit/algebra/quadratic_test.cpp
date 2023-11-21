#include "algebra.hpp"
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
	const auto ACTUAL_ROOTS = boost::math::tools::quadratic_roots(complex(1),
																  complex(2),
																  complex(3));

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

void unit_test() {
	/* using namespace std::complex_literals; */
	/* // Run tests where we know the complex roots that should be found. */

	/* // Quadratic solver tests. */
	/* test_known_quadratic_roots(-2.3 + 4.8i, */
	/* 						   +3.2 - 4.1i, */
	/* 						   -2.5 + 7.7i); // two distinct roots */
	/* test_known_quadratic_roots(+5.5 + 4.4i, */
	/* 						   +8.2 - 2.1i, */
	/* 						   +8.2 - 2.1i); // redundant pair of roots */

	/* // Cubic solver tests. */
	/* test_known_cubic_roots(1.0, 2.0, 3.0, 4.0); */
	/* test_known_cubic_roots(-2.3 + 4.8i, +3.2 - 4.1i, -2.5 + 7.7i, 53.0
	 * - 23.9i); */

	/* // Quartic solver tests. */
	/* test_known_quartic_roots(1.0, QuarticComplexCoeffs{2.0, 3.0, 4.0, 5.0});
	 */

	/* test_known_quartic_roots(1.0, QuarticComplexCoeffs{+3.2, -2.5, 53.0,
	 * -8.7}); */
	/* test_known_quartic_roots(-2.3 + 4.8i, */
	/* 						 QuarticComplexCoeffs{+3.2 - 4.1i, */
	/* 											  -2.5 + 7.7i, */
	/* 											  53.0 - 23.9i, */
	/* 											  -9.2 - 8.7i}); */
}
} // namespace Algebra
} // namespace raytracing
