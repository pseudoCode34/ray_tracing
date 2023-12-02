#ifndef __DDC_POLYHEDRA_H
#define __DDC_POLYHEDRA_H

#include "optics.hpp"
#include "solid_reorientable.hpp"
#include "triangle.hpp"

namespace raytracing {
namespace Imager {

// A box with rectangular faces, all of which are mutually perpendicular.
class Cuboid : public SolidObject_Reorientable {
public:
	explicit Cuboid(double half_width, double half_length, double half_height);

protected:
	IntersectionList object_space_append_all_intersections(
		const Vector &vantage, const Vector &direction) const override;

	bool object_space_contains(const Vector &point) const override;

private:
	const double HALF_WIDTH;  // faces at r = -a and r = +a.
	const double HALF_LENGTH; // faces at s = -b and s = +b.
	const double HALF_HEIGHT; // faces at t = -c and t = +c.
};

// 12 faces, 20 edges
class Dodecahedron : public TriangleMesh {
public:
	explicit Dodecahedron(const Vector &center, double scale, const Optics &optics);

private:
	void add_face(const PentagonPoints& pentagon, const Optics &optics,
				  double edge);
};

/*
 * 20 faces, 12 edges
 */
struct Icosahedron : TriangleMesh {
	Icosahedron(const Vector &center, double scale, const Optics &optics);
};
} // namespace Imager
} // namespace raytracing
#endif // __DDC_POLYHEDRA_H
