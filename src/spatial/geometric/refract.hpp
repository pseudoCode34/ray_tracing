#ifndef REFRACT_HPP
#define REFRACT_HPP
#include "spatial/primitive.hpp"

#include <gsl/gsl-lite.hpp>

namespace raytracing {
inline Vector3Df refract(Vector3fConstRef incident, Vector3fConstRef normal,
						 float eta) {
	gsl_Expects(incident.isUnitary());
	gsl_Expects(normal.isUnitary());

	float cos_incidence = incident.dot(normal);
	float cos2_refracted
		= 1.f - eta * eta * (1.f - cos_incidence * cos_incidence);
	// Total internal reflection
	if (cos2_refracted <= 0) return Vector3Df::Zero();

	return eta * incident
		   - (eta * cos_incidence + std::sqrt(cos2_refracted)) * normal;
}
} // namespace raytracing
#endif /* ifndef REFRACT_HPP */
