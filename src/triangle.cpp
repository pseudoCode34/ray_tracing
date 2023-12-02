#include "triangle.hpp"

#include "algebra.hpp"
#include "imager.hpp"
#include "solid_object.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <compare>
#include <fmt/core.h>
#include <optional>
#include <span>

namespace raytracing {
namespace Imager {

TriangleMesh::TriangleMesh(const Vector &center, bool is_fully_enclosed)
	: SolidObject(center, is_fully_enclosed) {}

void TriangleMesh::add_point(Vector &&point) {
	point_list_.push_back(std::move(point));
}

void TriangleMesh::add_quad(const QuadrilateralPoints &quad,
							const Optics &optics) {
	for (const auto &sub_triangle : quad.split())
		add_triangle(sub_triangle, optics);
}

void TriangleMesh::add_pentagon(const PentagonPoints &pentagon,
								const Optics &optics) {
	for (const auto &sub_triangle : pentagon.split())
		add_triangle(sub_triangle, optics);
}

void TriangleMesh::validate(const TrianglePoints &points) const {
	const auto &[a, b, c] = points;
	assert(a < point_list_.size() || b < point_list_.size()
		   || c < point_list_.size());
}

std::expected<Eigen::Vector3d, Algebra::EquationSystemResultType>
TriangleMesh::attempt_plane_intersection(const Vector &vantage,
										 const Vector &direction,
										 std::span<Vector, 3> points) {
	const Vector &a = points[0], &b = points[1], &c = points[2];
	Eigen::Matrix3d mat{{direction.x, a.x - b.x, a.x - c.x},
						{direction.y, a.y - b.y, a.y - c.y},
						{direction.z, a.z - b.z, a.z - c.z}};
	Eigen::Vector3d vec(-a.x - vantage.x, -a.y - vantage.y, -a.z - vantage.z);
	return Algebra::eq_sys_solve(mat, vec);
}

Vector TriangleMesh::get_point_from_index(int point_index) const {
	return point_list_[point_index];
}

// Adds another triangular facet to this solid object. aPointIndex,
// bPointIndex, cPointIndex are integer indices into the list of points
// already added. This allows a point to be referenced multiple times and
// have its location changed later without the caller having to change the
// (x,y,z) coordinates in multiple places. Each triangle may have its own
// optical properties (matte, gloss, refraction, opacity).
void TriangleMesh::add_triangle(const TrianglePoints &triangle,
								const Optics &optics) {
	validate(triangle);
	//  Cannot include the same point twice (indicates that it has no area).
	assert(
		triangle.all_unique_points()
		&& "Not allowed to use the same point index twice within a triangle.");

	triangle_list_.push_back({triangle, optics});
}

IntersectionList
TriangleMesh::append_all_intersections(const Vector &vantage,
									   const Vector &direction) const {
	IntersectionList intersection_list;
	auto has_any_root = [&vantage, &direction](auto &&point_list) {
		do {
			return attempt_plane_intersection(vantage, direction, point_list);
		} while (std::ranges::next_permutation(point_list).found);
	};
	auto map_to_point = [this](const Triangle &tri) {
		Vector a_point = point_list_[tri.points.a];
		Vector b_point = point_list_[tri.points.b];
		Vector c_point = point_list_[tri.points.c];

		// Sometimes we have to try more than one ordering of the points
		// (A,B,C) in order to get a valid solution to the intersection
		// equations.
		return std::to_array({a_point, b_point, c_point});
	};
	// Iterate through all the triangles in this solid object,
	// looking for every intersection.
	for (const Triangle &tri : triangle_list_) {
		auto points   = map_to_point(tri);
		auto has_root = has_any_root(points);
		if (!has_root.has_value()) continue;

		double u = has_root.value().x(), v = has_root.value().y(),
			   w = has_root.value().z();
		// We found an intersection of the direction with the plane that
		// passes through the points (A,B,C). Figure out whether the
		// intersection point is inside the triangle (A,B,C) or outside it.
		// We are interested only in intersections that are inside the
		// triangle. The trick here is that the values v,w are fractions
		// that will be 0..1 along the line segments AB and BC (or whichever
		// ordered triple of points we found the solution for). If we just
		// checked that both v and w are in the range 0..1, we would be
		// finding intersections with a parallelogram ABCD, where D is the
		// fourth point that completes the parallelogram whose other
		// vertices are ABC. But checking instead that v + w <= 1.0
		// constrains the set of points to the interior or border of the
		// triangle ABC.
		// We have found an intersection with one of the triangular
		// facets! Also determine whether the intersection point is in
		// "front" of the vantage (positively along the direction) by
		// checking for (u >= EPSILON). Note that we allow for a little
		// roundoff error by checking against EPSILON instead of 0.0,
		// because this method is called using vantage = a point on this
		// surface, in order to calculate surface lighting, and we don't
		// want to act like the surface is shading itself!
		if ((v >= 0.0) && (w >= 0.0) && (v + w <= 1.0) && (u >= EPSILON)) {
			const Vector DISPLACEMENT = u * direction;

			intersection_list.push_back(Intersection{
				.distance_squared = DISPLACEMENT.magnitude_squared(),
				.point            = vantage + DISPLACEMENT,
				.surface_normal   = normal_vector(tri),
				.solid            = this,
				.context          = &tri
				// remember which triangle we hit, for SurfaceOptics().
			});
		}
	}
	return intersection_list;
}

Vector TriangleMesh::normal_vector(const Triangle &triangle) const {
	// We could make this run faster if we cached the normal vector for each
	// triangle, but that will mean carefully remembering to update the
	// cache every time the vertex values in pointList are changed.

	// The normal vector is the normalized (unit magnitude) vector cross
	// product of the vectors AB and BC.  Because A,B,C are always
	// counterclockwise as seen from outside the solid surface, the
	// right-hand rule for cross products causes the normal vector to point
	// outward from the solid object.
	const Vector &a = point_list_[triangle.points.a];
	const Vector &b = point_list_[triangle.points.b];
	const Vector &c = point_list_[triangle.points.c];

	return (b - a).cross(c - b).unit_vector();
}

SolidObject &TriangleMesh::translate(double dx, double dy, double dz) {
	// chain to base class method, so that
	// center gets translated correctly.
	SolidObject::translate(dx, dy, dz);

	for (Vector &point : point_list_) {
		point.x += dx;
		point.y += dy;
		point.z += dz;
	}

	return *this;
}

SolidObject &TriangleMesh::rotate(double angle_in_degrees, char axis) {
	const auto [cos_val, sin_val]
		= Algebra::calculate_cos_sin(angle_in_degrees);
	const Vector CENTER = get_center();

	for (Vector &point : point_list_) {
		const auto [dx, dy, dz] = point - CENTER;

		switch (axis) {
		case 'x':
			point.y = CENTER.y + (cos_val * dy - sin_val * dz);
			point.z = CENTER.z + (cos_val * dz + sin_val * dy);
			break;
		case 'y':
			point.x = CENTER.x + (cos_val * dx - sin_val * dz);
			point.z = CENTER.z + (cos_val * dz + sin_val * dx);
			break;
		case 'z':
			point.x = CENTER.x + (cos_val * dx - sin_val * dy);
			point.y = CENTER.y + (cos_val * dy + sin_val * dx);
			break;
		default:
			// FIXME: Consider a throw here
			fmt::println(stderr, "Invalid axis specified.");
		}
	}

	return *this;
}

Optics TriangleMesh::surface_optics(const Vector &surface_point,
									const void *context) const {
	// Each triangular face may have different optics, so we take advantage
	// of the fact that 'context' is set to point to whichever Triangle the
	// ray intersected.
	const Triangle &triangle = *static_cast<const Triangle *>(context);
	return triangle.optics;
}

bool check_edge(const Vector &a, const Vector &b, double edge) {
	double distance = (b - a).magnitude();

	double error = std::abs((distance - edge) / edge);
	return error > EPSILON;
}
} // namespace Imager
} // namespace raytracing
