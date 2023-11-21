#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "algebra.hpp"
#include "intersection.hpp"
#include "optics.hpp"
#include "solid_object.hpp"
#include "vector.hpp"

#include <optional>
#include <span>
#include <stddef.h>
#include <vector>

namespace raytracing {
namespace Algebra {
struct LinearEquationSystemRoot;
} // namespace Algebra

namespace Imager {
/*
 * \brief A solid object consisting of nothing but triangular faces.
 *
 * Faces are added after construction by calling AddPoint() to append a series
 * of vertex points, followed by AddTriangle() to refer to the indices of
 * previously added points.
 */
class TriangleMesh : public SolidObject {
public:
	TriangleMesh(const Vector &center = {}, bool is_fully_enclosed = true);

	IntersectionList
	append_all_intersections(const Vector &vantage,
							 const Vector &direction) const override;

	SolidObject &translate(double dx, double dy, double dz) override;
	SolidObject &rotate_x(double angle_in_degrees) override;
	SolidObject &rotate_y(double angle_in_degrees) override;
	SolidObject &rotate_z(double angle_in_degrees) override;

	// Appends a new vertex point whose point index is to be
	// referenced later by AddTriangle.
	// expectedIndex is the zero-based value that must match
	// the insertion order of the point.
	// For example, the first call to AddPoint must pass
	// expectedIndex==0, the second must pass expectedIndex==1, etc.
	// This is a sanity check so that the caller avoids insertion
	// order mistakes, since the vertex point index will be
	// referenced later when calling AddTriangle.
	void add_point(int expected_index, double x, double y, double z);

	// Given the vertex point indices of three distinct points
	// that have already been added (using a call to AddPoint),
	// appends a new triangular face with those three points
	// as vertices.  Uses the specified optical properties for this face.
	void add_triangle(int a_point_index, int b_point_index, int c_point_index,
					  const Optics &optics);

	// A convenience method for cases where we know we have
	// a quadrilateral surface that can be split into two triangles.
	// The point indices (a,b,c,d) are passed in counterclockwise
	// order viewed from outside the surface.
	// This is important for calculating normal vectors that
	// point outward from the solid, not inward.
	void add_quad(int a_point_index, int b_point_index, int c_point_index,
				  int d_point_index, const Optics &optics);

	// Another convenience method for solids that
	// have pentagonal faces. A pentagon can be split into 3 triangles.
	// As in AddTriangle and AddQuad, the caller must pass the point
	// indices in a counterclockwise order as seen from outside the solid.
	void add_pentagon(int a_point_index, int b_point_index, int c_point_index,
					  int d_point_index, int e_point_index,
					  const Optics &optics);

	// Because each triangle can have different optics, we need
	// to override SurfaceOptics() to replace the default behavior
	// of having uniform optics for the whole solid.
	Optics surface_optics(const Vector &surface_point,
						  const void *context) const override;

protected:
	void validate_point_index(size_t point_index) const;

	// Attempts to find an intersection of the given direction
	// passing through the given vantage point with the plane
	// that passes through the triangle (A, B, C).
	// If an intersection can be found, returns true and sets
	// the output parameters:
	//
	//     u = The scalar multiple such that the intersection
	//         point = (u*direction + vantage).
	//
	//     v = The component of the vector difference B-A,
	//         starting at A, of the intersection point.
	//
	//     w = The component of the vector difference C-A,
	//         starting at A, of the intersection point.
	//
	// Returns false and leaves u, v, w undefined if found no intersection.
	// Note that it is possible that calling AttemptPlaneIntersection
	// may succeed with one ordering of the plane points (A, B, C),
	// but may fail on another ordering.
	static std::optional<Algebra::LinearEquationSystemRoot>
	attempt_plane_intersection(const Vector &vantage, const Vector &direction,
							   std::span<Vector, 3> points);

	Vector get_point_from_index(int point_index) const;

private:
	struct Triangle;

	// Returns a unit vector at right angles to the triangle,
	// using right-hand rule with respect to A,B,C ordering.
	Vector normal_vector(const Triangle &triangle) const;

	// Type definitions for convenient use of vector templates.
	using PointList    = std::vector<Vector>;
	using TriangleList = std::vector<Triangle>;

	// A list of all the vertex points used to define triangles.
	// A given point may be referenced by one or more triangles.
	PointList point_list_{};

	// A list of all the triangles, each of which refers
	// to 3 distinct points in pointList.
	TriangleList triangle_list_{};
};

struct TriangleMesh::Triangle {
	int a{};       // index into pointList for first  vertex of
				   // the triangle
	int b{};       // index into pointList for second vertex of
				   // the triangle
	int c{};       // index into pointList for third  vertex of
				   // the triangle

	Optics optics; // surface color of the triangle
};

} // namespace Imager
} // namespace raytracing
#endif /* ifndef TRIANGLE_HPP */
