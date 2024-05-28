#include "geometric.hpp"
#include "random.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

namespace raytracing {

SCENARIO("Reflectance test", "[reflectance]") {
	using Catch::Generators::random;
	using Catch::Matchers::WithinAbs;
	GIVEN("A ratio is presumed for entering refraction. From low ior to high "
		  "one") {
		float eta = GENERATE(take(10, random(0.f, 1.f)));

		AND_GIVEN("A normal vector and a light ray. Both are normalised") {
			const Vector3f incident = random_unit_vector();
			Vector3f normal         = random_unit_vector();

			if (bool is_internal_ray = incident.dot(normal) > 0;
				is_internal_ray) {
				normal = -normal;
				eta    = 1 / eta;
			}

			WHEN("reflectance() is called") {
				const float actual = reflectance(incident, normal, eta);

				THEN("Reflectance is the ratio within [0,1)") {
					CAPTURE(eta, incident, normal);
					REQUIRE_THAT(actual, WithinAbs(0.5, 0.5));
				}
			}
		}
	}
}
} // namespace raytracing
