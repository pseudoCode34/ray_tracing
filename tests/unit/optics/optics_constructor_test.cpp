#include "optics.hpp"

#include <gtest/gtest.h>

namespace raytracing::Imager {
TEST(OpticsConstructorTest, ValidConstruction) {
	ASSERT_NO_FATAL_FAILURE({ Optics under_test(Color("red")); });
	ASSERT_NO_FATAL_FAILURE({ Optics under_test(Color(100, 128, 128, 0.4)); });
}

TEST(OpticsConstructorDeathTest, InvalidConstruction) {
	EXPECT_DEATH(Optics under_test(Color{298, 100, 128});, "red");
	EXPECT_DEATH(Optics under_test(Color{"blue"}, Color{128, 200, 256});
				 , "blue");
	EXPECT_DEATH(Optics under_test(Color{"green"}, Color{"black"}, 1.2);
				 , "opacity");
}
} // namespace raytracing::Imager
