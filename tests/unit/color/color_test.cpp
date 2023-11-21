#include "color_test.hpp"

#include <math.h>

namespace raytracing {

TEST_F(ColorDeathTest, InvalidRGBAFormat) {
	ASSERT_DEATH({ Color color("cyan", 3.2); }, "alpha >= 0 && alpha <= 1");
	ASSERT_DEATH({ Color color(280, 100, 0); }, "red >= 0 && red <= 255");
	ASSERT_DEATH({ Color color(128, 30, 0, -3); }, "alpha >= 0 && alpha <= 1");
}

TEST_F(ColorDeathTest, ColorNameNotDefinedYet) {
	ASSERT_DEATH({ Color color("denim"); }, "not defined");
}

TEST_F(ColorDeathTest, IsEmptyInitially) {
	constexpr Color ZERO{};

	const Color COLOR2{255, 50, 50, 0.2}, COLOR3{0, 255, 255}, COLOR4{"blue"};

	EXPECT_NE(under_test, ZERO);
	EXPECT_NE(COLOR2, ZERO);
	EXPECT_NE(COLOR3, ZERO);
	EXPECT_NE(COLOR4, ZERO);
}

TEST_F(ColorDeathTest, AddOperation) {
	const Color ANOTHER_COLOR(0, 128, 200);
	const Color ACTUAL = under_test + ANOTHER_COLOR;

	EXPECT_EQ(ACTUAL.red, under_test.red);
	EXPECT_EQ(ACTUAL.green, 256);
	EXPECT_EQ(ACTUAL.blue, 455);
	EXPECT_EQ(ACTUAL.alpha, under_test.alpha);

	ASSERT_DEATH({ ACTUAL.validate(); }, "green >= 0 && green <= 255");
}

TEST_F(ColorDeathTest, ZeroMultiplication) {
	Color expected(0, 0, 0, under_test.alpha);
	Color actual = under_test * 0;
	EXPECT_EQ(actual, expected);
}

TEST_F(ColorDeathTest, IntScalarMultiplication) {
	int scalar         = 2;
	const Color ACTUAL = under_test * scalar;

	EXPECT_EQ(ACTUAL.red, 510);
	EXPECT_EQ(ACTUAL.green, 256);
	EXPECT_EQ(ACTUAL.blue, 510);
	EXPECT_EQ(ACTUAL.alpha, under_test.alpha);

	ASSERT_DEATH({ ACTUAL.validate(); }, "red >= 0 && red <= 255");
}

TEST_F(ColorDeathTest, FloatingPointScalarMultiplication) {
	double scalar      = 5.0 / 3;
	const Color ACTUAL = under_test * scalar;

	EXPECT_EQ(ACTUAL.red, 425);
	EXPECT_EQ(ACTUAL.green, 213);
	EXPECT_EQ(ACTUAL.blue, 425);
	EXPECT_EQ(ACTUAL.alpha, under_test.alpha);

	ASSERT_DEATH({ ACTUAL.validate(); }, "red >= 0 && red <= 255");
}

TEST_F(ColorDeathTest, ScalarDivision) {
	double scalar      = 5.0 / 3;
	const Color ACTUAL = under_test * scalar;

	EXPECT_EQ(ACTUAL.red, 425);
	EXPECT_EQ(ACTUAL.green, 213);
	EXPECT_EQ(ACTUAL.blue, 425);
	EXPECT_EQ(ACTUAL.alpha, under_test.alpha);
}
} // namespace raytracing
