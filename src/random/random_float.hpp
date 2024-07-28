#ifndef RANDOM_FLOAT_HPP
#define RANDOM_FLOAT_HPP

#include <concepts>
#include <gsl/gsl-lite.hpp>
#include <random>

namespace raytracing {

/**
 * \brief Returns a random number ranging between [from, upto) using uniform
 * distribution
 *
 * \note \a from must be less than \a upto
 */
template <std::floating_point T>
[[nodiscard]] inline T random_float(T from = static_cast<T>(0),
									T upto = static_cast<T>(1)) {
	gsl_Expects(from < upto);
	static std::mt19937 gen(std::random_device{}());

	static std::uniform_real_distribution<T> dist;
	using parm_t = decltype(dist)::param_type;
	return dist(gen, parm_t{from, upto});
}

} // namespace raytracing
#endif /* ifndef RANDOM_FLOAT_HPP */
