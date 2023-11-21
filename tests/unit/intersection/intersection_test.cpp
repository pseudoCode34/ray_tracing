#include "intersection_test.hpp"

namespace raytracing {
TEST_F(IntersectionAttributeTest, DistanceSquaredIsNotDefaultInitialised) {
	const double EXPECTED_INITIAL_VALUE = 1.0e+20;
	EXPECT_NE(under_test.distance_squared, EXPECTED_INITIAL_VALUE);
}

TEST_F(IntersectionAttributeTest, SolidObjectIsNotDefaultInitialised) {
	const auto EXPECTED_INITIAL_VALUE = nullptr;
	EXPECT_NE(under_test.solid, EXPECTED_INITIAL_VALUE);
}

TEST_F(IntersectionAttributeTest, PointIsNotDefaultInitialised) {
	constexpr Vector EXPECTED_INITIAL_VALUE{};
	EXPECT_NE(under_test.point, EXPECTED_INITIAL_VALUE);
}

TEST_F(IntersectionAttributeTest, ContextIsDefaultInitialised) {
	const auto EXPECTED_INITIAL_VALUE = nullptr;
	EXPECT_EQ(under_test.context, EXPECTED_INITIAL_VALUE);
}

TEST_F(IntersectionAttributeTest, TagIsDefaultInitialised) {
	const auto EXPECTED_INITIAL_VALUE = nullptr;
	EXPECT_EQ(under_test.tag, EXPECTED_INITIAL_VALUE);
}

} // namespace raytracing
