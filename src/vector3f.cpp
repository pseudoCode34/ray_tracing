#include "vector3f.hpp"

#include "floating_point_comparisons.hpp"

namespace raytracing {
float is_facing_forward(Vector3fConstRef normal, Vector3fConstRef incident) {
	float cos_incidence = incident.dot(normal);
	if (!FPC::is_positive(cos_incidence))
		return std::numeric_limits<float>::quiet_NaN();
	return cos_incidence;
}
} // namespace raytracing
