/*
	setunion.cpp

	Copyright (C) 2013 by Don Cross  -  http://cosinekitty.com/raytrace

	This software is provided 'as-is', without any express or implied
	warranty. In no event will the author be held liable for any damages
	arising from the use of this software.

	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute it
	freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you must not
	   claim that you wrote the original software. If you use this software
	   in a product, an acknowledgment in the product documentation would be
	   appreciated but is not required.

	2. Altered source versions must be plainly marked as such, and must not be
	   misrepresented as being the original software.

	3. This notice may not be removed or altered from any source
	   distribution.

	-------------------------------------------------------------------------
	Implements class SetUnion, the simple combination of two other solid
   objects.
*/

#include "intersection.hpp"
#include "set.hpp"
#include "solid_object.hpp"

#include <range/v3/core.hpp>
#include <range/v3/view/concat.hpp>

namespace raytracing {
namespace Imager {
struct Vector;

SetUnion::SetUnion(const Vector &center, SolidObject *left, SolidObject *right)
	: SolidObject_BinaryOperator(center, left, right) {}

IntersectionList
SetUnion::append_all_intersections(const Vector &vantage,
								   const Vector &direction) const {
	// Find all intersections with the left solid.
	auto x = left().append_all_intersections(vantage, direction);

	// Append all intersections with the right solid.
	auto y = right().append_all_intersections(vantage, direction);
	return ranges::views::concat(x, y) | ranges::to<IntersectionList>();
}

bool SetUnion::contains(const Vector &point) const {
	// A point is inside the set union if it is in either of the nested
	// solids.
	return left().contains(point) || right().contains(point);
}
} // namespace Imager
} // namespace raytracing
