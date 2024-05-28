//
// Created by Nguyễn Khắc Trường on 02/05/2024.
//
#include "material.hpp"
#include "point3f.hpp"
#include "ray.hpp"
#include "solid_object_list.hpp"
#include "sphere.hpp"
#include "vector3f.hpp"

#include <catch2/catch_test_macros.hpp>

namespace raytracing {
TEST_CASE("find_closest_intersection() function test",
		  "[solid_object_list][intersection][hit]") {
	GIVEN("A list of one ground sphere and one glass sphere above") {
		//  TODO: Mock these ones
		SolidObjectList world;
		world.insert(std::make_unique<Sphere>(
			Point3f{0, -100, 0},
			Material{ScaledColor(0, 0.3, 0.5), 1, 1, 1.3},
			100));
		world.insert(std::make_unique<Sphere>(
			Point3f{-4, 1, 0},
			Material{ScaledColor(1, 0, 0.2), 0.5, 0.7, 1.8},
			1.0));
		world.insert(std::make_unique<Sphere>(
			Point3f{3, 1, 0},
			Material{ScaledColor(0.5, 0.6, 0.8), 0.9, 0.85, 1.6},
			1.0));

		AND_GIVEN("A ray looking at pixel(335,425) that is a noisy white "
				  "on the output image") {
			const Ray r{Point3f{-1.99547529, -0.0364699364, -1.81954718},
						Vector3f{-0.687738955, -0.676333367, -0.263796031}};

			WHEN("find_closest_intersection() is called") {
				const auto intersection = world.find_closest_intersection(r);

				THEN("Expect the ray hits the ground") {
					const Point3f point = intersection->position;

					CAPTURE(world.container(), point);

					REQUIRE(intersection.has_value());

					REQUIRE(r.contains(point));
					REQUIRE(world[0].contains(point));

					REQUIRE(intersection->material == &world[0].get_optics());
				}
			}
		}
	}
}
} // namespace raytracing
