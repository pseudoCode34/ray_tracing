#include "set.hpp"

namespace raytracing {
namespace Imager {
SetDifference::SetDifference(const Vector &center, SolidObject *left,
							 SolidObject *right)
	: SetIntersection(center, left, new SetComplement(right)) {}

} // namespace Imager
} // namespace raytracing
