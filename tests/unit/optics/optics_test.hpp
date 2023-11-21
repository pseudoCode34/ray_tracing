#ifndef OPTICS_TEST_HPP
#define OPTICS_TEST_HPP

#include "optics.hpp"

#include <gtest/gtest.h>

namespace raytracing {
using Imager::Optics;
using Imager::Color;

struct OpticsTest : public testing::Test {
	void SetUp() override {}

	Optics under_test;
};
} // namespace raytracing
#endif /* ifndef OPTICS_TEST_HPP */
