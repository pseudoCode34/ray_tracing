#ifndef SOLID_REORIENTABLE_HPP
#define SOLID_REORIENTABLE_HPP
#include "solid_object.hpp"

namespace raytracing {
namespace Imager {
// This derived abstract class is specialized for objects (like torus)
// that are easy to define in terms of a fixed orientation and position
// in space, but for which generalized rotation makes the algebra
// annoyingly difficult. Instead, we allow defining the object in terms
// of a new coordinate system <r,s,t> and translate locations and rays
// from <x,y,z> camera coordinates into <r,s,t> object coordinates.
class SolidObject_Reorientable : public SolidObject {
public:
	explicit SolidObject_Reorientable(const Vector &center = {});

	/*
	 * \brief Returns the number of intersections found, which will have the
	 * same value as intersectionList.size().
	 *
	 * Fills in 'intersectionList' with a list of all the intersections found
	 * starting at the specified vantage point in the specified direction. Any
	 * pre-existing content in 'intersectionList' is discarded first.
	 */
	IntersectionList
	append_all_intersections(const Vector &vantage,
							 const Vector &direction) const override;
	SolidObject &rotate(double angle_in_degrees,
									  char axis) override;

	std::expected<bool, ContainmentError>
	contains(const Vector &point) const override;

	Optics surface_optics(const Vector &surface_point,
										const void *context) const override;

protected:
	// The following method is called by AppendAllIntersections, but with
	// 'vantage' and 'direction' vectors transformed from <x,y,z> camera space
	// into <r,s,t> object space. Intersection objects are returned in terms of
	// object coordinates, and they are automatically translated back into
	// camera coordinates by the caller.
	virtual IntersectionList
	object_space_append_all_intersections(const Vector &vantage,
										  const Vector &direction) const
		= 0;

	/*
	 * \brief Returns true if the specified point in object space is on or
	 * inside the solid object.
	 *
	 * Actually, well-behaved derived classes should provide a tolerance for
	 * points slightly outside the object's boundaries and return true then
	 * also. This tolerance handles small floating point rounding errors that
	 * may cause a point that is supposed to be considered part of the solid to
	 * be incorrectly excluded.
	 */
	virtual bool object_space_contains(const Vector &point) const = 0;

	virtual Optics object_space_surface_optics(const Vector &surface_point,
											   const void *context) const;

	Vector object_dir_from_camera_dir(const Vector &camera_dir) const;

	Vector object_point_from_camera_point(const Vector &camera_point) const;

	Vector camera_dir_from_object_dir(const Vector &object_dir) const;

	Vector camera_point_from_object_point(const Vector &object_point) const;

	void update_inverse_rotation();

private:
	// The members rDir, sDir, tDir are unit vectors in the direction of
	// the <r,s,t> object axes, each expressed in <x,y,z> camera space.
	// For any point P = <Px,Py,Pz> in camera coordinates, we can
	// determine object-relative coordinates as dot products
	// <(P-C).rDir,(P-C).sDir,(P-C).tDir>,
	// where C = the center of the object as returned by method Center().
	// Another way to look at this is that (rDir, sDir, tDir) taken
	// together are really just a 3*3 rotation matrix.
	Vector r_dir_;
	Vector s_dir_;
	Vector t_dir_;

	// The members xDir, yDir, zDir are unit vectors in the direction
	// of the <x,y,z> camera axes, each expressed in <r,s,t> object space.
	// These are maintained in tandem with rDir, sDir, tDir as various
	// rotations take place.  Taken together, they form an inverse
	// rotation matrix, so (xDir,yDir,zDir) as a 3*3 matrix
	// is calculated as the transpose of the 3*3 matrix (rDir,sDir,tDir).
	// Because an object is never rotated during the rendering of a given
	// frame, it is more efficient to have both matrices pre-calculated.
	Vector x_dir_;
	Vector y_dir_;
	Vector z_dir_;
};
} // namespace Imager
} // namespace raytracing
#endif /* ifndef SOLID_REORIENTABLE_HPP */
