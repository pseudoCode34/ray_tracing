//
// Created by Nguyễn Khắc Trường on 25/04/2024.
//

#include "material.hpp"
#include "point3f.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "vector3f.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_tostring.hpp>
#include <cmath>

namespace raytracing {


SCENARIO("Sphere hit member function tests", "[Sphere][Intersection][hit]") {
	using std::isinf;

	GIVEN("A solid_object located at (1,2,3) with the radius of 3") {
		const Sphere under_test(Point3f(1, 2, 3), Material{}, 3);

		AND_GIVEN("An external secant ray hit twice") {
			const Ray r{Point3f{-5, 0, 0}, Vector3f{5, 2, 3}};
			WHEN("hit() is called") {
				auto actual = under_test.hit(r);
				THEN("The closest intersection is found") {
					Point3f point = r.at(actual);
					REQUIRE(under_test.contains(point));
					REQUIRE(r.contains(point));
				}
			}
		}

		AND_GIVEN("An external secant ray that is almost tangent") {
			const Ray r{Point3f{-4, 0, 0}, Vector3f{3.71, 5.17, 3}};
			WHEN("hit() is called") {
				auto actual = under_test.hit(r);
				THEN("The closest intersection is found") {
					Point3f point = r.at(actual);
					REQUIRE(under_test.contains(point));
					REQUIRE(r.contains(point));
				}
			}
		}

		AND_GIVEN("The internal secant ray pointing inward") {
			const Ray r{Point3f{-2, 1, 2}, Vector3f{1, 1, 2}};
			WHEN("hit() is called") {
				auto actual = under_test.hit(r);
				THEN("The closest intersection is found") {
					Point3f point = r.at(actual);
					REQUIRE(under_test.contains(point));
					REQUIRE(r.contains(point));
				}
			}
		}

		AND_GIVEN("An external random ray impossible to cut") {
			const Ray r{Point3f{4, 1, 5}, Vector3f{1, 1, 2}};
			WHEN("hit() is called") {
				auto actual = under_test.hit(r);
				THEN("Found no intersection") { REQUIRE(isinf(actual)); }
			}
		}

		AND_GIVEN("A tangent ray on the surface") {
			const Ray r{Point3f{3, 0, 0}, Vector3f{0, 1, 0}};
			WHEN("hit() is called") {
				auto actual = under_test.hit(r);
				THEN("Not return the intersection with the same point") {
					REQUIRE(isinf(actual));
				}
			}
		}

		AND_GIVEN("An internal secant ray near the surface pointing inward ") {
			const Ray r{Point3f{3.98, 2, 3.03}, Vector3f{-1, 0, 1}};
			WHEN("hit() is called") {
				auto actual = under_test.hit(r);
				THEN("The closest intersection is found") {
					REQUIRE_FALSE(isinf(actual));
					Point3f point = r.at(actual);
					REQUIRE(under_test.contains(point));
					REQUIRE(r.contains(point));
				}
			}
		}
		AND_GIVEN("An internal secant ray near the surface pointing outward ") {
			const Ray r{Point3f{3.98, 2, 3.03}, Vector3f{1, 1, 2}};
			WHEN("hit() is called") {
				auto actual = under_test.hit(r);
				THEN("The closest intersection is found") {
					REQUIRE_FALSE(isinf(actual));
					Point3f point = r.at(actual);
					REQUIRE(under_test.contains(point));
					REQUIRE(r.contains(point));
				}
			}
		}

		AND_GIVEN("An invalid ray(direction is zero)") {
			const Ray r{Point3f{1, 0, 0}, Vector3f{0, 0, 0}};
			WHEN("hit() is called") {
				auto actual = under_test.hit(r);
				THEN("Found no intersection") { REQUIRE(isinf(actual)); }
			}
		}
	}
}
} // namespace raytracing
