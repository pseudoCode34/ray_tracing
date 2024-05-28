#include "vector3f.hpp"

#include <catch2/catch_test_macros.hpp>

namespace raytracing {

TEST_CASE("Eigen::Vector3f addition binary operation",
		  "[Vector3f][Eigen][add]") {
	// given
	const Vector3f lhs{1.0, 2.0, 3.0}, rhs{4.0, 5.0, 6.0},
		expected{5.0, 7.0, 9.0};
	// when
	const Vector3f actual = lhs + rhs;
	// then
	REQUIRE(actual.isApprox(expected));
}

TEST_CASE("Eigen::Vector3f subtraction binary operation",
		  "[Vector3f][Eigen][subtraction]") {
	// given
	const Vector3f lhs{1.0, 2.0, 3.0}, rhs{4.0, 5.0, 6.0},
		expected{-3.0, -3.0, -3.0};
	// when
	const Vector3f actual = lhs - rhs;
	// then
	REQUIRE(actual.isApprox(expected));
}


} // namespace raytracing
