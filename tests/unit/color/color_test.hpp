#ifndef COLOR_TEST_HPP
#define COLOR_TEST_HPP

#include "color.hpp"

#include <gtest/gtest.h>

namespace raytracing {
using Imager::Color;

struct ColorDeathTest : public testing::Test {
	void SetUp() final { under_test = {255, 128, 255,0.2}; }

	Color under_test;
};
} // namespace raytracing
#endif /* ifndef COLOR_TEST_HPP */
