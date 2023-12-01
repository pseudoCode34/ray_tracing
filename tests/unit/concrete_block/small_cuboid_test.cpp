#include "block.hpp"

#include <gtest/gtest.h>
#include <memory>

namespace raytracing {
namespace Imager {
TEST(SmallCuboidTest, Construction) {
	Optics optics{Color{"grey"}};
	std::unique_ptr<SolidObject> under_test(create_large_cuboid(optics));

	EXPECT_EQ(under_test->get_uniform_optics(), optics);
}

} // namespace Imager

} // namespace raytracing
