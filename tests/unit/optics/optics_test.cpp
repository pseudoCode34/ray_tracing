#include "optics_test.hpp"

namespace raytracing {
TEST_F(OpticsTest, Constructor) {
	ASSERT_NO_FATAL_FAILURE({ Optics under_test(Color("red")); });
}
} // namespace raytracing
