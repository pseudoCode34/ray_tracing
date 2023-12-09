/*
		icosahedron.cpp

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

		Implements the icosahedron - a 20-sided regular polyhedron.
		See:  http://en.wikipedia.org/wiki/Icosahedron
*/

#include "polyhedra.hpp"
#include "triangle.hpp"
#include "vector.hpp"

namespace raytracing::Imager {

Icosahedron::Icosahedron(const Vector &center, double scale,
						 const Optics &optics)
	: TriangleMesh(center) {
	// As explained in the Wikipedia article
	// http://en.wikipedia.org/wiki/Icosahedron under the "Cartesian
	// coordinates" section, the 12 vertex points are at
	//     (0,    +/-1, +/-p)
	//     (+/-1, +/-p,    0)
	//     (+/-p,    0, +/-1)
	// where p = (1 + sqrt(5))/2, also known as the Golden Ratio.
	// We adjust all of these to be clustered around the specified center
	// location.

	using std::numbers::phi;
	// Add the 12 vertices...

	add_point({center.x, center.y + scale, center.z + phi});
	add_point({center.x, center.y + scale, center.z - phi});
	add_point({center.x, center.y - scale, center.z + phi});
	add_point({center.x, center.y - scale, center.z - phi});

	add_point({center.x + scale, center.y + phi, center.z});
	add_point({center.x + scale, center.y - phi, center.z});
	add_point({center.x - scale, center.y + phi, center.z});
	add_point({center.x - scale, center.y - phi, center.z});

	add_point({center.x + phi, center.y, center.z + scale});
	add_point({center.x + phi, center.y, center.z - scale});
	add_point({center.x - phi, center.y, center.z + scale});
	add_point({center.x - phi, center.y, center.z - scale});

	// Add the 20 triangular faces.
	// I built a physical model of an icosahedron
	// and labeled the vertices using the point indices above.
	// I then labeled the faces with lowercase letters as
	// show in the comments for each add_triangle call below.
	// Each triplet of points in each add_triangle call is arranged
	// in counterclockwise order as seen from outside the icosahedron,
	// so that vector cross products will work correctly to
	// calculate the surface normal unit vectors for each face.

	add_triangle({2, 8, 0}, optics); // a
	add_triangle(
		{
			2,
			0,
			10,
		},
		optics); // b
	add_triangle(
		{
			2,
			10,
			7,
		},
		optics); // c
	add_triangle(
		{
			2,
			7,
			5,
		},
		optics); // d
	add_triangle(
		{
			2,
			5,
			8,
		},
		optics); // e

	add_triangle(
		{
			9,
			8,
			5,
		},
		optics); // f
	add_triangle(
		{
			9,
			5,
			3,
		},
		optics); // g
	add_triangle(
		{
			9,
			3,
			1,
		},
		optics); // h
	add_triangle(
		{
			9,
			1,
			4,
		},
		optics); // i
	add_triangle(
		{
			9,
			4,
			8,
		},
		optics); // j

	add_triangle(
		{
			6,
			11,
			10,
		},
		optics); // k
	add_triangle(
		{
			6,
			10,
			0,
		},
		optics); // l
	add_triangle(
		{
			6,
			0,
			4,
		},
		optics); // m
	add_triangle(
		{
			6,
			4,
			1,
		},
		optics); // n
	add_triangle(
		{
			6,
			1,
			11,
		},
		optics); // o

	add_triangle(
		{
			7,
			10,
			11,
		},
		optics); // p
	add_triangle(
		{
			7,
			11,
			3,
		},
		optics); // q
	add_triangle(
		{
			7,
			3,
			5,
		},
		optics); // r
	add_triangle(
		{
			0,
			8,
			4,
		},
		optics); // s
	add_triangle(
		{
			1,
			3,
			11,
		},
		optics); // t
}
} // namespace raytracing::Imager
