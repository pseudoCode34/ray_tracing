#include "binary.hpp"
#include "color.hpp"
#include "intersection.hpp"
#include "optics.hpp"
#include "refraction_constants.hpp"
#include "solid_object.hpp"
#include "vector.hpp"

namespace raytracing::Imager {

SolidObject::SolidObject(const Vector &center, bool is_fully_enclosed)
	: center_(center), is_fully_enclosed_(is_fully_enclosed) {}

IntersectionResult
SolidObject::find_closest_intersection(const Vector &vantage,
									   const Vector &direction) const {
	cached_intersection_list_ = append_all_intersections(vantage, direction);
	return pick_closest_intersection(cached_intersection_list_);
}

double SolidObject::get_refractive_index() const { return refractive_index_; }

Optics SolidObject::surface_optics(const Vector &surface_point,
								   const void *context) const {
	return uniform_optics_;
}

SolidObject &SolidObject::translate(double dx, double dy, double dz) {
	center_.x += dx;
	center_.y += dy;
	center_.z += dz;
	return *this;
}

SolidObject &SolidObject::move_point_to(double cx, double cy, double cz) {
	translate(cx - center_.x, cy - center_.y, cz - center_.z);
	return *this;
}

SolidObject &SolidObject::move_centre_to(const Vector &new_center) {
	move_point_to(new_center.x, new_center.y, new_center.z);
	// TODO: Inst this *this = new_center?
	return *this;
}

std::expected<bool, ContainmentError>
SolidObject::contains(const Vector &point) const {
	// FIXME: This function does not handle the "corner case":
	// multiple intersections found at the same point but for
	// different facets of the solid.

	if (!is_fully_enclosed_)
		// Whoever constructed this object has indicated that it should not be
		// considered to contain any points.
		return std::unexpected(ContainmentError::MISSING_BOUNDARY);

	// This method assumes that the solid's surfaces fully enclose a volume of
	// space without any gaps or cracks. Pick an arbitrary direction in space
	// and count the number of times we enter and exit this solid.
	const Vector DIRECTION{0.0, 0.0, 1.0};

	enclosure_list_ = append_all_intersections(point, DIRECTION);

	int enter_count = 0; // number of times we enter the solid
	int exit_count  = 0; // number of times we exit the solid

	for (const Intersection &intersection : enclosure_list_) {
		// Calculate the dot product of the direction with
		// the surface normal.
		const double DOTPROD = DIRECTION.dot(intersection.surface_normal);

		// If it is positive, we are exiting the solid.
		// If it is negative, we are entering the solid.
		if (DOTPROD > EPSILON) ++exit_count;
		else if (DOTPROD < -EPSILON) ++enter_count;
		else
			// If the dot product is too close to zero, something odd is going
			// on because we should not have found an intersection with a plane
			// in the first place.
			return std::unexpected(ContainmentError::AMBIGUOUS_TRANSISTION);
	}

	// If the original point is within this solid, we have exited the object one
	// more time than we entered. Otherwise, we have exited and entered the same
	// number of times.
	switch (exit_count - enter_count) {
	case 0: return false;

	case 1: return true;

	default:
		// This can happen only if the solid's surfaces do not properly enclose
		// a volume of space without gaps. Either the surfaces need to be
		// corrected so as to perfectly seal the interior volume or this
		// instance should be constructed with isFullyEnclosed initialized to
		// false.
		return std::unexpected(ContainmentError::INDETERMINABLE);
	}
}

void SolidObject::set_matte_gloss_balance(double gloss_factor,
										  const Color &raw_matte_color,
										  const Color &raw_gloss_color) {
	uniform_optics_
		= balance_matte_gloss(gloss_factor, raw_matte_color, raw_gloss_color);
}

void SolidObject::set_full_matte(const Color &matte_color) {
	uniform_optics_ = balance_matte_gloss(
		0.0,      // glossFactor = 0 indicates full matte reflection
		matte_color,
		Color{}); // irrelevant, but must pass something
}

void SolidObject::set_opacity(double opacity) {
	uniform_optics_.set_opacity(opacity);
}

void SolidObject::set_refraction(double refraction) {
	refraction::validate_refraction(refraction);
	refractive_index_ = refraction;
}

const Optics &SolidObject::get_uniform_optics() const {
	return uniform_optics_;
}

SolidObject_BinaryOperator::SolidObject_BinaryOperator(const Vector &center,
													   SolidObject *left,
													   SolidObject *right)
	: SolidObject(center), left_(left), right_(right) {}

SolidObject &SolidObject_BinaryOperator::left() const { return *left_; }

SolidObject &SolidObject_BinaryOperator::right() const { return *right_; }
} // namespace raytracing::Imager
