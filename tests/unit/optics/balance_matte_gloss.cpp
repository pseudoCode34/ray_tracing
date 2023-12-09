#include "optics.hpp"

#include "gtest/gtest.h"

namespace raytracing::Imager {
struct ColorPair {
	Color matte, gloss;
};

TEST(BalanceMatteGlossTest, EdgeCase) {
	// given
	const double GLOSS_FACTOR = 1.0;
	const Color MATTE_COLOR{"green"};
	const ColorPair EXPECTED{.matte = Color("black"), .gloss = Color("white")};

	// when
	Optics under_test = balance_matte_gloss(GLOSS_FACTOR, MATTE_COLOR);

	// then
	EXPECT_EQ(under_test.get_matte_color(), EXPECTED.matte);
	EXPECT_EQ(under_test.get_gloss_color(), EXPECTED.gloss);
}

TEST(BalanceMatteGlossTest, BasicCase) {
	// given
	const double GLOSS_FACTOR = 0.3;
	const ColorPair INPUT{.matte = Color("green"), .gloss = Color("blue")};
	const ColorPair EXPECTED{.matte = Color(0, 89, 0),
							 .gloss = Color(0, 0, 76)};

	// when
	Optics under_test
		= balance_matte_gloss(GLOSS_FACTOR, INPUT.matte, INPUT.gloss);

	// then
	EXPECT_EQ(under_test.get_matte_color(), EXPECTED.matte);
	EXPECT_EQ(under_test.get_gloss_color(), EXPECTED.gloss);
}
} // namespace raytracing::Imager
