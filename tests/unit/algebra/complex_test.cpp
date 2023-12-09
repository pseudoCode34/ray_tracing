#include "algebra_test.hpp"
#include "common_constants.hpp"

#include <gtest/gtest.h>

namespace raytracing::Algebra {
TEST(ComplexTest, ComplexNear) {
	// given
	std::complex<double> a(1.0, 2.0000001);
	std::complex<double> b(1.0000004, 2.0);

	// then
	EXPECT_THAT(a, ComplexNear(b, EPSILON));
}

} // namespace raytracing::Algebra
