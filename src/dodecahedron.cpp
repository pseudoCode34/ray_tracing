/*
		dodecahedron.cpp

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

		2. Altered source versions must be plainly marked as such, and must not
   be misrepresented as being the original software.

		3. This notice may not be removed or altered from any source
		   distribution.

		-------------------------------------------------------------------------

		Implements the dodecahedron - a 12-sided regular polyhedron.
		See:  http://en.wikipedia.org/wiki/Dodecahedron
*/

#include "polyhedra.hpp"
#include "triangle.hpp"
#include "vector.hpp"

#include <numbers>

namespace raytracing {
namespace Imager {
class Optics;

Dodecahedron::Dodecahedron(const Vector &center, double s, const Optics &optics)
	: TriangleMesh(center) {
	// Set up constants p, r that are sized proportionally to scale parameter s.
	using std::numbers::phi; // Golden ratio
	const double p    = s * phi;
	const double r    = s / phi;
	const double edge = r * 2;

	// Using the specified center point, arrange the 20 vertices around it.
	add_point({center.x - r, center.y + p, center.z});
	add_point({center.x + r, center.y + p, center.z});
	add_point({center.x + s, center.y + s, center.z - s});
	add_point({center.x, center.y + r, center.z - p});
	add_point({center.x - s, center.y + s, center.z - s});
	add_point({center.x - s, center.y + s, center.z + s});
	add_point({center.x, center.y + r, center.z + p});
	add_point({center.x + s, center.y + s, center.z + s});
	add_point({center.x + p, center.y, center.z + r});
	add_point({center.x + p, center.y, center.z - r});
	add_point({center.x + s, center.y - s, center.z - s});
	add_point({center.x, center.y - r, center.z - p});
	add_point({center.x - s, center.y - s, center.z - s});
	add_point({center.x - p, center.y, center.z - r});
	add_point({center.x - p, center.y, center.z + r});
	add_point({center.x - s, center.y - s, center.z + s});
	add_point({center.x, center.y - r, center.z + p});
	add_point({center.x + s, center.y - s, center.z + s});
	add_point({center.x + r, center.y - p, center.z});
	add_point({center.x - r, center.y - p, center.z});

	// Define the 12 pentagonal faces of the dodecahedron.
	add_face({0, 1, 2, 3, 4}, optics, edge);      // a
	add_face({0, 5, 6, 7, 1}, optics, edge);      // b
	add_face({1, 7, 8, 9, 2}, optics, edge);      // c
	add_face({2, 9, 10, 11, 3}, optics, edge);    // d
	add_face({3, 11, 12, 13, 4}, optics, edge);   // e
	add_face({4, 13, 14, 5, 0}, optics, edge);    // f
	add_face({5, 14, 15, 16, 6}, optics, edge);   // g
	add_face({6, 16, 17, 8, 7}, optics, edge);    // h
	add_face({8, 17, 18, 10, 9}, optics, edge);   // i
	add_face({10, 18, 19, 12, 11}, optics, edge); // j
	add_face({12, 19, 15, 14, 13}, optics, edge); // k
	add_face({19, 18, 17, 16, 15}, optics, edge); // l
}

void Dodecahedron::add_face(const PentagonPoints &pentagon,
							const Optics &optics, double edge) {
	// This method is a thin wrapper for AddPentagon that adds some sanity
	// checking. It makes sure that the edges AB, BC, CD, DE, EA are all of the
	// same length. This is to help me track down bugs.

	const auto &[a, b, c, d, e] = pentagon;
	// FIXME: does this prototype make any sense?
	assert(check_edge(get_point_from_index(a), get_point_from_index(b), edge));
	assert(check_edge(get_point_from_index(b), get_point_from_index(c), edge));
	assert(check_edge(get_point_from_index(c), get_point_from_index(d), edge));
	assert(check_edge(get_point_from_index(d), get_point_from_index(e), edge));
	assert(check_edge(get_point_from_index(e), get_point_from_index(a), edge));

	add_pentagon(pentagon, optics);
}
} // namespace Imager
} // namespace raytracing
