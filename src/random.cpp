#include "random.hpp"

#include <gsl/gsl-lite.hpp>
#include <random>

namespace raytracing {
float random_float(float from, float upto) {
	gsl_Expects(from < upto);
	static std::mt19937 gen(std::random_device{}());

	static std::uniform_real_distribution<float> dist;
	using parm_t = decltype(dist)::param_type;
	return dist(gen, parm_t{from, upto});
}

Vector3f random_vector(float from, float upto) {
	return Vector3f::NullaryExpr(
		[from, upto] { return random_float(from, upto); });
}

Vector3f random_unit_vector(float from, float upto) {
	return Vector3f::NullaryExpr(
			   [from, upto] { return random_float(from, upto); })
		.normalized();
}

Eigen::Vector2f random_in_unit_disk() {
	using Eigen::Vector2f;
	while (true) {
		Vector2f p = Vector2f::NullaryExpr([] { return random_float(-1, 1); });

		if (p.squaredNorm() < 1.f) return p;
	}
}

} // namespace raytracing
