#ifndef INTERSECTION_HPP
#define INTERSECTION_HPP

#include "material.hpp"

#include <vector>

namespace raytracing {

/**
 * \brief Describe where the ray hits any surface in the scene.
 *
 * The position, together with the normalised local vector of the object. A
 * non-owning pointer to that very solid object. The parametric root is for
 * considering what is the closest intersection to a particular point.
 */
struct Intersection {
	Point3Df position = Point3Df::Zero();
	Vector3Df normal  = -Vector3Df::UnitZ();

	Material *material;

	auto operator==(const Intersection &other) const noexcept {
		return position.isApprox(other.position)
			   && normal.isApprox(other.normal);
	}

	[[nodiscard]] Point3Df shift(float epsilon) const {
		return position + normal * epsilon;
	}
};

using IntersectionList = std::vector<Intersection>;
} // namespace raytracing
#endif /* ifndef INTERSECTION_HPP */
