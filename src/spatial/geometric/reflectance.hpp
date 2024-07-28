#ifndef REFLECTANCE_HPP
#define REFLECTANCE_HPP

#include "spatial/primitive.hpp"
#include "utility.hpp"

#include <gsl/gsl-lite.hpp>

namespace raytracing {
inline float reflectance(Vector3fConstRef incident, Vector3fConstRef normal,
						 float eta) {
	gsl_Expects(incident.isUnitary());
	gsl_Expects(normal.isUnitary());

	float cos_incidence = -incident.dot(normal);

	float r0 = (1 - eta) / (1 + eta);
	float reflection_coeff
		= std::lerp(std::pow(1.f - cos_incidence, 5.f), 1.f, r0 * r0);
	gsl_Ensures(within_range(reflection_coeff));

	return reflection_coeff;
}

inline ScaledColor lerp(float t, ScaledColorConstRef low,
						ScaledColorConstRef high) {
	gsl_Expects(within_range(t));
	return low * (1 - t) + high * t;
}


} // namespace raytracing
#endif /* ifndef REFLECTANCE_HPP */
