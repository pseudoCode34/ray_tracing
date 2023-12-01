#include "optics_test.hpp"

#include "gtest/gtest.h"

namespace raytracing {
TEST_F(OpticsTest, Constructor) {
	ASSERT_NO_FATAL_FAILURE({ Optics under_test(Color("red")); });
	EXPECT_DEATH({ Optics under_test(Color{298, 100, 128}); }, "red");
	EXPECT_DEATH(
		{
			Optics under_test(Color{"blue"}, Color{128, 200, 256});
		},
		"blue");
	EXPECT_DEATH({ Optics under_test(Color{"green"}, Color{"black"}, 1.2); },
				 "opacity");
}

TEST_F(OpticsTest, BalanceMatteGloss) {
	// given
	Optics under_test(Color("red"));

	// when
	under_test.balance_matte_gloss(1.0, Color{"green"});

	// then
	EXPECT_EQ(under_test.get_matte_color(), Color{"black"});
	EXPECT_EQ(under_test.get_gloss_color(), Color{"white"});
}
} // namespace raytracing
