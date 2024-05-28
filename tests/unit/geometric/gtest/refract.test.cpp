
#include "geometric.hpp"
#include "vector3f.hpp"

#include <cmath>
#include <gtest/gtest.h>
#include <numbers>

namespace raytracing {
TEST(RefractDeathTest, InternalRay) {
	// given A ratio indicating exiting refraction") {
	float eta            = random_float(1, 2);
	float critical_angle = std::asin(1 / eta);

	// A normal vector and a light ray near the normal of the surface") {
	Vector3f incident, normal;
	float incident_angle;
	do {
		incident = random_unit_vector(), normal = random_unit_vector();
		incident_angle = std::acos(incident.dot(normal));
	} while (incident_angle > critical_angle);

	// THEN("The light ray is inside the object") {
	EXPECT_TRUE(incident_angle < std::numbers::pi / 2);

	// AND_THEN("Incident and normal are normalised vectors") {
	EXPECT_TRUE(incident.isUnitary());
	EXPECT_TRUE(normal.isUnitary());

	// THEN("Angle of incidence is less than critical_angle") {
	EXPECT_TRUE(incident_angle < critical_angle);

	//	WHEN("refract() is called") {
	EXPECT_DEBUG_DEATH(
		{ const Vector3f refracted_dir = refract(incident, normal, eta); },
		"Assertion")
		<< incident.transpose() << normal.transpose() << eta;
}
} // namespace raytracing
