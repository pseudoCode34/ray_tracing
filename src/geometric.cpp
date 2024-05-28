#include "geometric.hpp"

#include "floating_point_comparisons.hpp"

#include <gsl/gsl-lite.hpp>

namespace raytracing {
float reflectance(Vector3fConstRef incident, Vector3fConstRef normal,
				  float eta) {
	gsl_Expects(incident.isUnitary());
	gsl_Expects(normal.isUnitary());

	float cos_incidence = -incident.dot(normal);

	float r0 = (1 - eta) / (1 + eta);
	float reflection_coeff
		= std::lerp(std::pow(1.f - cos_incidence, 5.f), 1.f, r0 * r0);
	gsl_Ensures(0 <= reflection_coeff && reflection_coeff <= 1);

	return reflection_coeff;
}

ScaledColor lerp(float t, const ScaledColor &low, const ScaledColor &high) {
	gsl_Expects(t <= 1 && t >= 0);
	return low * (1 - t) + high * t;
}

Vector3f reflect(Vector3fConstRef incident, Vector3fConstRef normal) {
	gsl_Expects(incident.isUnitary());
	gsl_Expects(normal.isUnitary());

	using Mat3f = Eigen::Matrix3f;
	const Mat3f householder
		= Mat3f::Identity() - 2.f * normal * normal.transpose();
	return householder * incident;
}

Vector3f refract(Vector3fConstRef incident, Vector3fConstRef normal,
				 float eta) {
	gsl_Expects(incident.isUnitary());
	gsl_Expects(normal.isUnitary());

	float cos_incidence = incident.dot(normal);
	float cos2_refracted
		= 1.f - eta * eta * (1.f - cos_incidence * cos_incidence);
	// Total internal reflection
	if (!FPC::is_positive(cos2_refracted)) return Vector3f::Zero();

	return eta * incident
		   - (eta * cos_incidence + std::sqrt(cos2_refracted)) * normal;
}
} // namespace raytracing
