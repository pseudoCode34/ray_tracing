#ifndef PICK_CLOSEST_INTERSECTION_TEST_HPP
#define PICK_CLOSEST_INTERSECTION_TEST_HPP
#include "sphere.hpp"
#include "vector.hpp"

#include <gtest/gtest.h>

namespace raytracing {
namespace Imager {
struct ClosestIntersectionTest : public testing::Test {
	void SetUp() final {
		sphere1 = {Vector(), 0.72};
		sphere2 = {Vector{0.0, 0.0, -17.0}, 0.72};
		sphere3 = {Vector{+1.8, 0.0, -17.0}, 0.72};
	}

	// FIXME: Mock these things
	Sphere sphere1, sphere2, sphere3;
};
} // namespace Imager
} // namespace raytracing
#endif /* PICK_CLOSEST_INTERSECTION_TEST_HPP */
