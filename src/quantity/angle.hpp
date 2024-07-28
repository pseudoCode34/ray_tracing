#ifndef ANGLE_HPP
#define ANGLE_HPP

// IWYU pragma: private, include "quantity.hpp"
#include <mp-units/quantity.h>
#include <mp-units/systems/angular/angular.h>

namespace raytracing {

using DegreeAnglef = mp_units::quantity<mp_units::angular::degree, float>;

/**
 * Customising tan for mp-units angular quantity is to return a float for doing
 * math calculations with Eigen3 Scalar type, not the dimensionless quantity
 * returned by mp-units::angular::tan.
 *
 * \param angle A strongly precision-preserving type. Value in other angular
 * units are implicitly converted without precision loss, especially if followed
 * by an integral representation type.
 *
 * \note mp-units::angular::tan()
 */
template <mp_units::QuantityOf<mp_units::angular::angle> Angle>
constexpr Angle::rep tan(Angle angle) {
	using mp_units::angular::radian;

	return std::tan(angle.numerical_value_in(radian));
}
} // namespace raytracing
#endif /* ifndef ANGLE_HPP */
