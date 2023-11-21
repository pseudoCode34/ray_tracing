#include "algebra_test.hpp"

#include <gtest/gtest.h>

namespace raytracing {
namespace Algebra {
TEST(ComplexTest, ComplexEquality) {
	// given
	std::complex<double> a(1.0, 2.0);
	std::complex<double> b(1.0, 2.0);

	// then
	EXPECT_THAT(a, ComplexNear(b, EPSILON));
}

} // namespace Algebra
} // namespace raytracing
