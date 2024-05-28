//
// Created by Nguyễn Khắc Trường on 04/05/2024.
//

#include "geometric.hpp"
#include "vector3f.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <cmath>
#include <numbers>

namespace raytracing {

SCENARIO("Refracted ray test with an internal light ray",
		 "[refraction][exiting]") {
	using Catch::Generators::random, Catch::Matchers::IsNaN,
		Catch::Matchers::WithinAbs;
	using std::asin, std::sin, std::acos, std::fabs;
	using std::numbers::pi;

	GIVEN("A ratio indicating exiting refraction") {
		float eta            = GENERATE(take(5, random(1.f, 2.f)));
		float critical_angle = asin(1 / eta);

		AND_GIVEN(
			"A normal vector and a light ray near the normal of the surface") {
			Vector3f incident, normal;
			float incident_angle;
			do {
				incident = random_unit_vector(), normal = random_unit_vector();
				incident_angle = std::acos(incident.dot(normal));
			} while (incident_angle > critical_angle);

			THEN("The light ray is inside the object") {
				CHECK(incident_angle < pi / 2);
			}

			THEN("Angle of incidence is less than critical_angle") {
				CHECK(incident_angle < critical_angle);

				WHEN("refract() is called") {
					const Vector3f refracted_dir
						= refract(incident, -normal, eta);
					float actual_refracted = acos(refracted_dir.dot(normal));

					THEN("Reflected ray exits the object") {
						CAPTURE(incident.transpose(),
								normal.transpose(),
								eta,
								refracted_dir.transpose(),
								incident_angle,
								actual_refracted);

						const float refracted_angle_expected
							= asin(eta * sin(incident_angle));
						REQUIRE_THAT(refracted_angle_expected, !IsNaN());
						REQUIRE_FALSE(refracted_dir.isZero());

						REQUIRE_THAT(
							actual_refracted,
							WithinAbs(refracted_angle_expected, 1e-3f));
						REQUIRE(incident_angle < actual_refracted);
					}
				}
			}
		}
	}
}

SCENARIO("Specical case of total internal reflection in refraction test",
		 "[refraction][internal_reflection][exiting]") {
	using Catch::Generators::random, Catch::Matchers::WithinAbs,
		Catch::Matchers::IsNaN;
	using std::asin, std::sin, std::acos, std::fabs;
	using std::numbers::pi;
	float eta            = GENERATE(take(5, random(1.f, 2.f)));
	float critical_angle = std::asin(1 / eta);

	AND_GIVEN("A normal vector and a light ray near the surface of 2 media") {
		Vector3f incident, normal;
		float incident_angle;
		do {
			incident = random_unit_vector(), normal = random_unit_vector();
			incident_angle = acos(incident.dot(normal));
		} while (incident_angle < critical_angle || incident_angle > pi / 2);

		THEN("The light ray is inside the object") {
			CHECK(incident_angle < pi / 2);
		}

		WHEN("Angle of incidence exceeds critical_angle") {
			CHECK(incident_angle > critical_angle);

			AND_WHEN("refract() is called") {
				const Vector3f refracted_dir = refract(incident, -normal, eta);
				float actual_refracted       = acos(refracted_dir.dot(normal));

				THEN("Total internal reflection happens") {
					CAPTURE(incident,
							normal,
							eta,
							refracted_dir,
							incident_angle,
							actual_refracted);

					const float refracted_angle_expected
						= asin(eta * sin(incident_angle));
					REQUIRE_THAT(refracted_angle_expected, IsNaN());
					REQUIRE(refracted_dir.isZero());
				}
			}
		}
	}
}

SCENARIO("Refracted ray test with an external incident ray and a normal vector "
		 "pointing outward",
		 "[refraction][entering]") {
	using Catch::Generators::random, Catch::Matchers::WithinAbs,
		Catch::Matchers::IsNaN;
	using std::asin, std::sin, std::acos, std::fabs;

	GIVEN("A normal vector and a light ray.Both are normalised") {
		Vector3f incident, normal;
		float cos_incidence;
		do {
			incident = random_unit_vector(), normal = random_unit_vector();
			cos_incidence = incident.dot(normal);
		} while (cos_incidence > 0);

		THEN("The light ray is outside the surface") {
			CHECK(cos_incidence < 0);
		}

		AND_GIVEN(
			"Light travels from low refractive index medium to high one") {
			float eta = GENERATE(take(5, random(0.f, 1.f)));

			const float refracted_angle_expected
				= asin(eta * sin(acos(cos_incidence)));

			THEN("eta is ratio of source medium index to the target one. Not "
				 "the mathematic ratio in reverse") {
				CHECK(eta < 1);
			}

			WHEN("refract() is called") {
				const Vector3f refracted_dir = refract(incident, normal, eta);
				float actual_refracted       = acos(-refracted_dir.dot(normal));

				THEN("No total internal reflection") {
					CAPTURE(incident,
							normal,
							eta,
							refracted_dir,
							cos_incidence,
							actual_refracted);

					CHECK_FALSE(refracted_dir.isZero());
					CHECK_THAT(refracted_angle_expected, !IsNaN());

					AND_THEN("Refracted ray is deflected towards the normal of "
							 "two media") {
						REQUIRE_THAT(
							actual_refracted,
							WithinAbs(refracted_angle_expected, 1e-3f));
						REQUIRE(acos(fabs(cos_incidence)) > actual_refracted);
					}
				}
			}
		}
	}
}
} // namespace raytracing
