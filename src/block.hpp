#ifndef __DDC_BLOCK_H
#define __DDC_BLOCK_H

#include "optics.hpp"
#include "set.hpp"
#include "solid_object.hpp"
#include "vector.hpp"

namespace raytracing::Imager {
class Optics;
class SolidObject;
struct Vector;

// A ConcreteBlock is a large cuboid with the union of two smaller cuboids
// subtracted from it. ConcreteBlock = SetDifference(LargeCuboid,
// SetUnion(SmallCuboid1, SmallCuboid2))

SolidObject *create_large_cuboid(const Optics &optics);
SolidObject *create_small_cuboid_union(const Optics &optics);

class ConcreteBlock : public SetDifference {
public:
	explicit ConcreteBlock(const Vector &center, const Optics &optics);
};
} // namespace raytracing::Imager

#endif // __DDC_BLOCK_H
