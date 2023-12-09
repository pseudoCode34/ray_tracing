#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "intersection.hpp"
#include "optics.hpp"
#include "solid_object.hpp"
#include "vector.hpp"

#include <array>
#include <cstddef>
#include <span>
#include <vector>

namespace raytracing::Imager {
struct TrianglePoints {
	size_t a, b, c;

	[[nodiscard]] constexpr bool all_unique_points() const {
		return a != b && b != c && c != a;
	}
};

struct QuadrilateralPoints {
	size_t a, b, c, d;

	[[nodiscard]] std::array<TrianglePoints, 2> split() const {
		// We preserve counterclockwise ordering by making two triangles:
		// (a,b,c) and (c,d,a).
		return std::to_array(
			{TrianglePoints{a, b, c}, TrianglePoints{c, d, a}});
	}
};

struct PentagonPoints {
	size_t a, b, c, d, e;

	[[nodiscard]] std::array<TrianglePoints, 3> split() const {
		return std::to_array({TrianglePoints{a, b, c},
							  TrianglePoints{c, d, e},
							  TrianglePoints{d, e, a}});
	}
};

bool check_edge(const Vector &a, const Vector &b, double edge);

/*
 * \brief A solid object consisting of nothing but triangular faces.
 *
 * Faces are added after construction by calling AddPoint() to append a
 * series of vertex points, followed by AddTriangle() to refer to the
 * indices of previously added points.
 */
class TriangleMesh : public SolidObject {
public:
	TriangleMesh(const Vector &center = {}, bool is_fully_enclosed = true);

	IntersectionList
	append_all_intersections(const Vector &vantage,
							 const Vector &direction) const override;

	SolidObject &translate(double dx, double dy, double dz) override;
	SolidObject &rotate(double angle_in_degrees, char axis) override;

	// Appends a new vertex point whose point index is to be referenced later by
	// AddTriangle.
	void add_point(Vector &&point);

	/*
	 * Given the \param vertex point indices of three distinct points added
	 * using a call to AddPoint, append a new triangular face with those
	 * three points as vertices. Use the specified \param optical properties
	 * for this face.
	 */
	void add_triangle(const TrianglePoints &triangle, const Optics &optics);

	// A convenience method for cases where we know we have
	// a quadrilateral surface that can be split into two triangles.
	// The point indices (a,b,c,d) are passed in counterclockwise
	// order viewed from outside the surface.
	// This is important for calculating normal vectors that
	// point outward from the solid, not inward.
	void add_quad(const QuadrilateralPoints &quad, const Optics &optics);

	// Another convenience method for solids that
	// have pentagonal faces. A pentagon can be split into 3 triangles.
	// As in AddTriangle and AddQuad, the caller must pass the point
	// indices in a counterclockwise order as seen from outside the solid.
	void add_pentagon(const PentagonPoints &pentagon, const Optics &optics);

	// Because each triangle can have different optics, we need
	// to override SurfaceOptics() to replace the default behavior
	// of having uniform optics for the whole solid.
	Optics surface_optics(const Vector &surface_point,
						  const void *context) const override;

protected:
	void validate(const TrianglePoints &points) const;

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
	static std::expected<Eigen::Vector3d, Algebra::EquationSystemResultType>
	attempt_plane_intersection(const Vector &vantage, const Vector &direction,
							   std::span<Vector, 3> points);

	Vector get_point_from_index(int point_index) const;

private:
	struct Triangle {
		TrianglePoints points{};
		Optics optics; // surface color of the triangle
	};

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

} // namespace raytracing::Imager
#endif /* ifndef TRIANGLE_HPP */
