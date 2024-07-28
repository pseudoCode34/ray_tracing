//
// Created by Nguyễn Khắc Trường on 27/04/2024.
//

#include "spatial/geometric/reflect.hpp"

#include "random.hpp"
#include "spatial/primitive.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

namespace raytracing {
TEST_CASE("Reflected ray test", "[reflection][internal_ray]") {
	// given
	const Vector3Df incident = random_unit_vector<float, 3>(),
					normal   = random_unit_vector<float, 3>();
	float cos_incidence      = incident.dot(normal);

	// when
	const Vector3Df actual = reflect(incident, normal);
	float cos_reflected    = actual.dot(normal);

	// then
	CAPTURE(incident, normal, actual, cos_incidence, cos_reflected);
	REQUIRE_THAT(cos_reflected + cos_incidence,
				 Catch::Matchers::WithinAbs(0, 1e-3));
}
} // namespace raytracing
