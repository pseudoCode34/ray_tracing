#include "pick_closest_intersection_test.hpp"

#include "intersection.hpp"

#include <gtest/gtest.h>

namespace raytracing::Imager {
TEST_F(ClosestIntersectionTest, OnlyOneFound) {
	// given
	const IntersectionList LIST{{.distance_squared = 0.51839999999999986,
								 .point          = Vector{-0.13033443409209908,
                                                 0.077911028379499231,
                                                 -0.70380594409733499},
								 .surface_normal = Vector{-0.18102004735013763,
														  0.1082097616381934,
														  -0.97750825569074318},
								 .solid          = &sphere1}};

	// when
	const auto &[actual_closest, type] = pick_closest_intersection(LIST);

	// then
	EXPECT_EQ(type, IntersectionResultType::UNIQUE);

	const Intersection &expected = LIST.front();
	EXPECT_EQ(actual_closest, expected);
}

TEST_F(ClosestIntersectionTest, NotFound) {
	// given
	const IntersectionList LIST;

	// when
	const auto &[actual_closest, type] = pick_closest_intersection(LIST);

	// then
	EXPECT_EQ(type, IntersectionResultType::NOT_FOUND);

	const Intersection EXPECTED{};
	EXPECT_EQ(actual_closest, EXPECTED);
}

TEST_F(ClosestIntersectionTest,
	   DISABLED_AmbiguityInMultipleIntersectionsFound) {
	// given
	const IntersectionList LIST{};

	// when


	// then
}

TEST_F(ClosestIntersectionTest, AcceptedResult) {
	// given
	const IntersectionList LIST{
		{.distance_squared = 0.51840000000000008,
		 .point = Vector{0, -0.0088882115628019613, -0.71994513658695891},
		 .surface_normal
		 = Vector{0, -0.012344738281669388, -0.99992380081522059},
		 .solid = &sphere1},
		{.distance_squared = 312.84558812285928,
		 .point            = {0, -0.21834683664591839, -17.686093768319392},
		 .surface_normal   = {0, -0.30325949534155844, -0.95290801155472682},
		 .solid            = &sphere2},
		{.distance_squared = 266.01504607403183,
		 .point            = {0, -0.20134228252506603, -16.308724884530349},
		 .surface_normal   = {0, -0.27964205906259643, 0.96010432704119786},
		 .solid            = &sphere3}};

	// when
	const auto &[actual_closest, type] = pick_closest_intersection(LIST);

	// then
	EXPECT_EQ(type, IntersectionResultType::ACCEPTABLE);

	const Intersection &expected = LIST.front();
	EXPECT_EQ(actual_closest, expected);
}

} // namespace raytracing::Imager
