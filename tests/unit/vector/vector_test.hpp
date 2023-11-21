#ifndef VECTOR_TEST_HPP
#define VECTOR_TEST_HPP

#include "vector.hpp"

#include <gtest/gtest.h>
#include <math.h>

namespace raytracing {
using Imager::Vector;

struct VectorTest : public testing::Test {
	void SetUp() final {
		vector1 = Vector{1.0, 2.0, 3.0};
		vector2 = Vector{-8.0 / 5, 1.0 / sqrt(3), 3.0 * exp(4)};
		vector3 = Vector{log(3), -log10(28), 3 * pow(9, -5)};
	}

	// void TearDown() override {}

	Vector vector1, vector2, vector3, vector4;
};
} // namespace raytracing
#endif /* ifndef VECTOR_TEST_HPP */
