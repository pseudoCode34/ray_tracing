#ifndef REFLECT_HPP
#define REFLECT_HPP
#include "spatial/primitive.hpp"

#include <gsl/gsl-lite.hpp>

namespace raytracing {
inline Vector3Df reflect(Vector3fConstRef incident, Vector3fConstRef normal) {
	gsl_Expects(incident.isUnitary());
	gsl_Expects(normal.isUnitary());

	using Mat3f = Eigen::Matrix3f;
	const Mat3f householder
		= Mat3f::Identity() - 2.f * normal * normal.transpose();
	return householder * incident;
}
} // namespace raytracing
#endif /* ifndef REFLECT_HPP */
