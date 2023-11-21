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

	2. Altered source versions must be plainly marked as such, and must not be
	   misrepresented as being the original software.

	3. This notice may not be removed or altered from any source
	   distribution.

	-------------------------------------------------------------------------

	Implements the dodecahedron - a 12-sided regular polyhedron.
	See:  http://en.wikipedia.org/wiki/Dodecahedron
*/

#include "imager.hpp"
#include "polyhedra.hpp"
#include "triangle.hpp"
#include "vector.hpp"

#include <math.h>
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
	add_point(0, center.x - r, center.y + p, center.z);
	add_point(1, center.x + r, center.y + p, center.z);
	add_point(2, center.x + s, center.y + s, center.z - s);
	add_point(3, center.x, center.y + r, center.z - p);
	add_point(4, center.x - s, center.y + s, center.z - s);
	add_point(5, center.x - s, center.y + s, center.z + s);
	add_point(6, center.x, center.y + r, center.z + p);
	add_point(7, center.x + s, center.y + s, center.z + s);
	add_point(8, center.x + p, center.y, center.z + r);
	add_point(9, center.x + p, center.y, center.z - r);
	add_point(10, center.x + s, center.y - s, center.z - s);
	add_point(11, center.x, center.y - r, center.z - p);
	add_point(12, center.x - s, center.y - s, center.z - s);
	add_point(13, center.x - p, center.y, center.z - r);
	add_point(14, center.x - p, center.y, center.z + r);
	add_point(15, center.x - s, center.y - s, center.z + s);
	add_point(16, center.x, center.y - r, center.z + p);
	add_point(17, center.x + s, center.y - s, center.z + s);
	add_point(18, center.x + r, center.y - p, center.z);
	add_point(19, center.x - r, center.y - p, center.z);

	// Define the 12 pentagonal faces of the dodecahedron.
	add_face(0, 1, 2, 3, 4, optics, edge);      // a
	add_face(0, 5, 6, 7, 1, optics, edge);      // b
	add_face(1, 7, 8, 9, 2, optics, edge);      // c
	add_face(2, 9, 10, 11, 3, optics, edge);    // d
	add_face(3, 11, 12, 13, 4, optics, edge);   // e
	add_face(4, 13, 14, 5, 0, optics, edge);    // f
	add_face(5, 14, 15, 16, 6, optics, edge);   // g
	add_face(6, 16, 17, 8, 7, optics, edge);    // h
	add_face(8, 17, 18, 10, 9, optics, edge);   // i
	add_face(10, 18, 19, 12, 11, optics, edge); // j
	add_face(12, 19, 15, 14, 13, optics, edge); // k
	add_face(19, 18, 17, 16, 15, optics, edge); // l
}

void Dodecahedron::add_face(int a_point_index, int b_point_index,
							int c_point_index, int d_point_index,
							int e_point_index, const Optics &optics,
							double edge) {
	// This method is a thin wrapper for AddPentagon that adds some sanity
	// checking. It makes sure that the edges AB, BC, CD, DE, EA are all of the
	// same length. This is to help me track down bugs.

	check_edge(a_point_index, b_point_index, edge);
	check_edge(b_point_index, c_point_index, edge);
	check_edge(c_point_index, d_point_index, edge);
	check_edge(d_point_index, e_point_index, edge);
	check_edge(e_point_index, a_point_index, edge);

	add_pentagon(a_point_index,
				 b_point_index,
				 c_point_index,
				 d_point_index,
				 e_point_index,
				 optics);
}

void Dodecahedron::check_edge(int a_point_index, int b_point_index,
							  double edge) const {
	// Look at two consecutive vertices that bound a face.
	const Vector A = get_point_from_index(a_point_index);
	const Vector B = get_point_from_index(b_point_index);

	// The vector difference between the two points represents the edge between
	// them.
	const Vector diff = B - A;

	// The length of the edge should match the expected value as passed in the
	// 'edge' parameter.
	const double distance = diff.magnitude();

	// If the error is more than one part in a million, something is definitely
	// wrong!
	const double error = fabs((distance - edge) / edge);
	if (error > 1.0e-6)
		throw ImagerException("Dodecahedron edge is incorrect.");
}
} // namespace Imager
} // namespace raytracing
