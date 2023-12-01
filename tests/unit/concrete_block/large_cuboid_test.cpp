#include "block.hpp"
#include "vector.hpp"

#include <gtest/gtest.h>

namespace raytracing {

namespace Imager {
TEST(LargeCuboidTest, Constructor) {
	ASSERT_NO_FATAL_FAILURE(
		ConcreteBlock block(Vector{0.0, 0.0, -3.0}, Optics(Color("grey"))););
}

TEST(LargeCuboidTest, RotateX) {
	ConcreteBlock block(Vector{0.0, 0.0, -3.0}, Optics(Color("grey")));
	block.rotate(-10.0, 'x');
	/* ConcreteBlock::create_large_cuboid(Optics(Color{"grey"})); */
	/* EXPECT_EQ(); */
}
} // namespace Imager
} // namespace raytracing
