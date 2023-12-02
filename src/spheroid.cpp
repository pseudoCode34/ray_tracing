#include "spheroid.hpp"

#include "vector.hpp"

#include <boost/math/tools/quartic_roots.hpp>
#include <math.h>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/transform.hpp>
#include <vector>

namespace raytracing {
namespace Imager {

Spheroid::Spheroid(double x_axis_radius, double y_axis_radius,
				   double z_axis_radius)
	: SolidObject_Reorientable(),
	  X_AXIS_RADIUS(x_axis_radius),
	  Y_AXIS_RADIUS(y_axis_radius),
	  Z_AXIS_RADIUS(z_axis_radius),
	  X_AXIS_RADIUS_SQR(x_axis_radius * x_axis_radius),
	  Y_AXIS_RADIUS_SQR(y_axis_radius * y_axis_radius),
	  Z_AXIS_RADIUS_SQR(z_axis_radius * z_axis_radius) {}

bool Spheroid::object_space_contains(const Vector &point) const {
	/* TODO <2023-08-25 10:11, John Williams>: Rename this vector */
	Vector r{point.x / X_AXIS_RADIUS,
			 point.y / Y_AXIS_RADIUS,
			 point.z / Y_AXIS_RADIUS};
	return r.magnitude_squared() <= 1.0 + EPSILON;
}

// The surface normal vector was calculated by expressing the spheroid as a
// function z(x,y) = sqrt(1 - (x/a)^2 - (y/b)^2), taking partial derivatives
// dz/dx = (c*c*x)/(a*a*z), dz/dy = (c*c*y)/(b*b*z), and using these to
// calculate the vectors <1, 0, dz/dx> and <0, 1, dy,dz>. The normalized cross
// product of these two vectors yields the surface normal vector.
Vector get_surface_vector(double x_axis_radius_sqr, double y_axis_radius_sqr,
						  double z_axis_radius_sqr,
						  const Vector &intersection_point) {
	const auto [x, y, z] = intersection_point;
	Vector surface_normal;
	if (fabs(z) <= EPSILON)
		if (fabs(x) <= EPSILON)
			// The equation devolves to (y^2)/(b^2) = 1, or y = +/- b.
			surface_normal = Vector{0.0, ((y > 0.0) ? 1.0 : -1.0), 0.0};
		else
			// The equation devolves to an ellipse on the xy plane :
			// (x^2)/(a^2) + (y^2)/(b^2) = 1.
			surface_normal
				= Vector{-1.0,
						 (-x_axis_radius_sqr * y) / (y_axis_radius_sqr * x),
						 0.0}
					  .unit_vector();
	else
		surface_normal
			= Vector{(z_axis_radius_sqr * x) / (x_axis_radius_sqr * z),
					 (z_axis_radius_sqr * y) / (y_axis_radius_sqr * z),
					 1.0}
				  .unit_vector();
	// Handle special cases with polarity: the polarity of the
	// components of the surface normal vector must match that
	// of the <x,y,z> intersection point, because the surface
	// normal vector always points (roughly) away from the
	// vantage, just like any point on the surface of the
	// spheroid does.

	// negative product means opposite polarities
	if (x * surface_normal.x < 0.0) surface_normal.x *= -1.0;
	if (y * surface_normal.y < 0.0) surface_normal.y *= -1.0;
	if (z * surface_normal.z < 0.0) surface_normal.z *= -1.0;
	return surface_normal;
}

IntersectionList
Spheroid::object_space_append_all_intersections(const Vector &vantage,
												const Vector &direction) const {
	using boost::math::tools::quadratic_roots;

	const auto [x0, x1] = quadratic_roots(
		Y_AXIS_RADIUS_SQR * Z_AXIS_RADIUS_SQR * direction.x * direction.x
			+ X_AXIS_RADIUS_SQR * Z_AXIS_RADIUS_SQR * direction.y * direction.y
			+ X_AXIS_RADIUS_SQR * Y_AXIS_RADIUS_SQR * direction.z * direction.z,
		2.0
			* (Y_AXIS_RADIUS_SQR * Z_AXIS_RADIUS_SQR * vantage.x * direction.x
			   + X_AXIS_RADIUS_SQR * Z_AXIS_RADIUS_SQR * vantage.y * direction.y
			   + X_AXIS_RADIUS_SQR * Y_AXIS_RADIUS_SQR * vantage.z
					 * direction.z),
		Y_AXIS_RADIUS_SQR * Z_AXIS_RADIUS_SQR * vantage.x * vantage.x
			+ X_AXIS_RADIUS_SQR * Z_AXIS_RADIUS_SQR * vantage.y * vantage.y
			+ X_AXIS_RADIUS_SQR * Y_AXIS_RADIUS_SQR * vantage.z * vantage.z
			- X_AXIS_RADIUS_SQR * Y_AXIS_RADIUS_SQR * Z_AXIS_RADIUS_SQR);

	auto make_intersection
		= [this, &vantage, &direction] [[nodiscard]] (double solution) {
			  Vector displacement = solution * direction,
					 location     = vantage + displacement;
			  return Intersection{
				  .distance_squared = displacement.magnitude_squared(),
				  .point            = location,
				  .surface_normal   = get_surface_vector(X_AXIS_RADIUS_SQR,
                                                       Y_AXIS_RADIUS_SQR,
                                                       Z_AXIS_RADIUS_SQR,
                                                       location),
				  .solid            = this};
		  };
	IntersectionList list;
	if (x0 > EPSILON) list.push_back(make_intersection(x0));
	if (x1 > EPSILON) list.push_back(make_intersection(x1));
	return list;
}
} // namespace Imager
} // namespace raytracing
