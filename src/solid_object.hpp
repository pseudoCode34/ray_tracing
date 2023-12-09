#ifndef SOLID_OBJECT_HPP
#define SOLID_OBJECT_HPP

#include "intersection.hpp"
#include "optics.hpp"
#include "refraction_constants.hpp"

#include <expected>

namespace raytracing::Imager {
enum class ContainmentError {
	MISSING_BOUNDARY,
	AMBIGUOUS_TRANSISTION,
	INDETERMINABLE
};

class SolidObject {
public:
	explicit SolidObject(const Vector &center   = {},
						 bool is_fully_enclosed = true);

	SolidObject(const SolidObject &)            = default;
	SolidObject(SolidObject &&)                 = delete;
	SolidObject &operator=(const SolidObject &) = default;
	SolidObject &operator=(SolidObject &&)      = delete;
	virtual ~SolidObject()                      = default;

	/*
	 * \brief Appends to 'intersectionList' all the intersections found starting
	 * at the specified vantage point in the direction of the direction vector.
	 *
	 * \param vantage A position vector that specifies a location in space from
	 * which the ray is drawn.
	 * \param direction A vector indicates which way the ray is aimed.
	 * \param intersection_list Zero or more Intersection describing the
	 * locations (if any) where the ray intersects with the opaque exterior
	 * surfaces of the solid object are generated and appended to the list
	 * parameter intersectionList, which may already contain other intersections
	 * before.
	 */
	virtual IntersectionList
	append_all_intersections(const Vector &vantage,
							 const Vector &direction) const
		= 0;

	/*
	 * \brief Search for any intersections with this solid from the vantage
	 * point in the given direction.
	 *
	 * \returns 0 If none are found and the 'intersection' parameter is left
	 * unchanged.
	 *
	 * \returns int the positive number of intersections that lie at minimal
	 * distance from the vantage point in that direction.
	 *
	 * Usually this number will be 1 (a unique intersection is closer than all
	 * the others) but it can be greater if multiple intersections are equally
	 * close (e.g. the ray hitting exactly at the corner of a cube could cause
	 * this function to return 3).
	 *
	 * \note If this function returns a value greater than zero, it means the
	 * 'intersection' parameter has been filled instd::expected<bool,
	 * ContainmentError> with the closest intersection (or one of the equally
	 * closest intersections).
	 */
	IntersectionResult find_closest_intersection(const Vector &vantage,
												 const Vector &direction) const;

	/*
	 * \brief Returns true if the given point is inside this solid object.
	 *
	 * This is a default implementation that counts intersections that enter or
	 * exit the solid in a given direction from the point. Derived classes can
	 * often implement a more efficient algorithm to override this default
	 * algorithm.
	 */
	virtual std::expected<bool, ContainmentError>
	contains(const Vector &point) const;

	/*
	 * \brief Returns the optical properties (reflection and refraction) at a
	 * given point on the surface of this solid.
	 *
	 * By default, the optical properties are the same everywhere, but a derived
	 * class may override this behavior to create patterns of different colors
	 * or gloss. It is recommended to keep constant refractive index throughout
	 * the solid, or the results may look weird.
	 */
	virtual Optics surface_optics(const Vector &surface_point,
								  const void *context) const;

	// Returns the index of refraction of this solid.
	// The refractive index is uniform throughout the solid.
	double get_refractive_index() const;

	// The following three member functions rotate this
	// object counterclockwise around a line parallel
	// to the x, y, or z axis, as seen from the positive
	// axis direction.
	virtual SolidObject &rotate(double angle_in_degrees, char axis) = 0;

	// Moves the entire solid object by the delta values dx, dy, dz. Derived
	// classes that override this method must chain to it in order to translate
	// the center of rotation.
	virtual SolidObject &translate(double dx, double dy, double dz);

	// Moves the center of the solid object to the new location (cx, cy, cz).
	SolidObject &move_point_to(double cx, double cy, double cz);

	// Moves the center of the solid object to the
	// location specified by the position vector newCenter.
	SolidObject &move_centre_to(const Vector &new_center);

	const Vector &get_center() const;

	// Derived classes are allowed to report different optical
	// properties at different points on their surface(s).
	// For example, different points might have different matte
	// colors in order to depict some kind of pattern on the solid.
	// However, by default, solids have uniform optical properites
	// everywhere on their surface(s).  This method allows callers
	// that know a solid to have this default behavior to define
	// the uniform optical properties.  All writes must take place
	// before rendering starts in order to avoid weird/ugly results.
	void set_uniform_optics(const Optics &optics);

	// Helper method for solids with uniform optics (see above).
	// Defines the shiny reflection color (gloss) and
	// dull reflection color (matte), with glossFactor = 0..1
	// that balances between the two modes of reflection.
	// If glossFactor = 0, the object has a completely dull
	// surface.  If glossFactor = 1, the surface is completely
	// mirror-like.
	void set_matte_gloss_balance(double gloss_factor,
								 const Color &raw_matte_color = Color("white"),
								 const Color &raw_gloss_color = Color("white"));

	void set_full_matte(const Color &matte_color);

	void set_opacity(double opacity);

	void set_refraction(double refraction);

	const Optics &get_uniform_optics() const;

private:
	Vector center_; // The point in space about which this object rotates.

	/*
	 * By default, a solid object has uniform optical properties across its
	 * entire surface. Unless a derived class overrides the virtual member
	 * function SurfaceOptics(), the member variable uniformOptics holds these
	 * optical properties.
	 */
	Optics uniform_optics_{};

	// A solid object has a uniform refractive index throughout its contained
	// volume.
	double refractive_index_{refraction::GLASS};

	// A flag that indicates whether the Contains() method should try to
	// determine whether a point is inside this solid.  If true, containment
	// calculations proceed; if false, Contains() always returns false.  Many
	// derived classes will override the Contains() method and therefore make
	// this flag irrelevant.
	bool is_fully_enclosed_;

	// The following members are an optimization to minimize the overhead and
	// fragmentation caused by repeated memory allocations creating and
	// destroying std::vector contents.
	mutable IntersectionList cached_intersection_list_;
	mutable IntersectionList enclosure_list_;
};
} // namespace raytracing::Imager

#endif /*ifndef SOLID_OBJECT_HPP*/
