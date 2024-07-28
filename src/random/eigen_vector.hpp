#ifndef RANDOM_EIGEN_VECTOR_HPP
#define RANDOM_EIGEN_VECTOR_HPP

#include "random_float.hpp"

#include <concepts>
#include <Eigen/Core>

namespace raytracing {
/**
 * \brief A custom random function returning an Eigen::Vector3f using C++11
 * std::uniform_real_distribution, instead of rand() in
 * Eigen::Vector3f::Random(). Furthermore, to specify a user-defined range of
 * values, instead of only [0,1).
 * */
template <std::floating_point T, int Size>
[[nodiscard]] inline Eigen::Vector<T, Size>
random_vector(T from = static_cast<T>(0), T upto = static_cast<T>(1)) {
	return Eigen::Vector<T, Size>::NullaryExpr(
		[from, upto] { return random_float(from, upto); });
}

/**
 * \brief Returns a random normalised vector
 */
template <std::floating_point T, int Size>
[[nodiscard]] inline Eigen::Vector<T, Size>
random_unit_vector(T from = static_cast<T>(-1), T upto = static_cast<T>(1)) {
	return Eigen::Vector<T, Size>::NullaryExpr(
			   [from, upto] { return random_float(from, upto); })
		.normalized();
}

} // namespace raytracing
#endif /* ifndef RANDOM_EIGEN_VECTOR_HPP */
