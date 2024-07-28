#ifndef PIXEL_HPP
#define PIXEL_HPP

// IWYU pragma: private, include "quantity.hpp"
#include <mp-units/systems/usc/usc.h>

namespace raytracing {

// NOLINTBEGIN(readability-identifier-naming)
inline constexpr struct pixel final
	: mp_units::named_unit<"px", mp_units::mag<mp_units::ratio{1, 96}>
									 * mp_units::usc::inch> {
} pixel;

// NOLINTEND(readability-identifier-naming)

} // namespace raytracing
#endif /* ifndef PIXEL_HPP */
