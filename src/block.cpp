#include "block.hpp"

#include "polyhedra.hpp"
#include "vector.hpp"

namespace raytracing::Imager {

// TODO: Move semantics here work with dynamic allocating?
ConcreteBlock::ConcreteBlock(const Vector &center, const Optics &optics)
	: SetDifference(Vector{}, create_large_cuboid(optics),
					create_small_cuboid_union(optics)) {
	move_centre_to(center);
}

SolidObject *create_large_cuboid(const Optics &optics) {
	auto *cuboid = new Cuboid(8.0, 16.0, 8.0);
	cuboid->set_uniform_optics(optics);
	return cuboid;
}

SolidObject *create_small_cuboid_union(const Optics &optics) {
	// We add a little bit in the z dimension (8.01 instead of 8.0)
	// to guarantee completely engulfing the hole.

	auto *top_cuboid    = new Cuboid(6.0, 6.5, 8.01);
	auto *bottom_cuboid = new Cuboid(6.0, 6.5, 8.01);

	top_cuboid->set_uniform_optics(optics);
	bottom_cuboid->set_uniform_optics(optics);

	top_cuboid->move_point_to(0.0, +7.5, 0.0);
	bottom_cuboid->move_point_to(0.0, -7.5, 0.0);

	return new SetUnion(Vector{}, top_cuboid, bottom_cuboid);
}
} // namespace raytracing::Imager
