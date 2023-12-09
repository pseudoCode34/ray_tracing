#ifndef INTERSECTION_TEST_HPP
#define INTERSECTION_TEST_HPP

#include "intersection.hpp"
#include "solid_object.hpp"
#include "sphere.hpp"
#include "vector.hpp"

#include <gtest/gtest.h>

namespace raytracing::Imager {
struct IntersectionAttributeTest : public testing::Test {
	void SetUp() final {
		object = new Sphere(Vector{0, 1, 0}, 1.2);
		under_test
			= Intersection{140.3, Vector{-2, 4, 3}, Vector{-3, 4, 5}, object};
	}

	void TearDown() final { delete object; }

	SolidObject *object;
	Intersection under_test;
};
} // namespace raytracing::Imager
#endif /* ifndef INTERSECTION_TEST_HPP */
