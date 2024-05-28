#ifndef QUANTITY_HPP
#define QUANTITY_HPP
#include <mp-units/dimension.h>
#include <mp-units/quantity.h>
#include <mp-units/quantity_spec.h>
#include <mp-units/systems/angular/angular.h>
#include <mp-units/unit.h>

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
constexpr auto tan(mp_units::QuantityOf<mp_units::angular::angle> auto angle)
	-> decltype(angle)::rep {
	using mp_units::angular::radian;

	return std::tan(angle.numerical_value_in(radian));
}
} // namespace raytracing
#endif /* ifndef QUANTITY_HPP */
