#include "block.hpp"
#include "vector.hpp"

#include <gtest/gtest.h>

namespace raytracing::Imager {
TEST(LargeCuboidDeathTest, DISABLED_Constructor) {
	ASSERT_NO_FATAL_FAILURE(
		ConcreteBlock block(Vector{0.0, 0.0, -3.0}, Optics(Color("grey"))););
}

TEST(LargeCuboidTest, DISABLED_RotateX) {
	ConcreteBlock block(Vector{0.0, 0.0, -3.0}, Optics(Color("grey")));
	// FIXME: how to test this private method? Convert it to free function?
	/* ConcreteBlock::create_large_cuboid(Optics(Color{"grey"})); */
	/* EXPECT_EQ(); */
}
} // namespace raytracing::Imager
