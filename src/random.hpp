#ifndef RANDOM_HPP
#define RANDOM_HPP

#include "vector3f.hpp"

namespace raytracing {

/**
 * \brief Returns a random number ranging between [from, upto) using uniform
 * distribution
 *
 * \note \a from must be less than \a upto
 */
float random_float(float from = 0.f, float upto = 1.f);

/**
 * \brief A custom random function returning an Eigen::Vector3f using C++11
 * std::uniform_real_distribution, instead of rand() in
 * Eigen::Vector3f::Random(). Furthermore, to specify a user-defined range of
 * values, instead of only [0,1).
 * */
Vector3f random_vector(float from = 0.f, float upto = 1.f);

/**
 * \brief Returns a random normalised vector
 */
Vector3f random_unit_vector(float from = -10.f, float upto = 10.f);

Eigen::Vector2f random_in_unit_disk();
} // namespace raytracing
#endif /* ifndef RANDOM_HPP */
