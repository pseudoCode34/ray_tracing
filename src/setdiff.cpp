#include "set.hpp"

#include <memory>

namespace raytracing {
namespace Imager {
SetDifference::SetDifference(const Vector &center, SolidObject *left,
							 SolidObject *right)
	: SetIntersection(center, left, new SetComplement(right)) {}

} // namespace Imager
} // namespace raytracing
