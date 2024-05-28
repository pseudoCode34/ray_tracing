#ifndef INTERSECTION_HPP
#define INTERSECTION_HPP

#include "material.hpp"
#include "point3f.hpp"
#include "vector3f.hpp"

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
	Point3f position = Point3f::Zero();
	Vector3f normal  = -Vector3f::UnitZ();

	Material *material;

	auto operator==(const Intersection &other) const noexcept {
		if (!position.isApprox(other.position, 1e-2f)) return false;
		if (!normal.isApprox(other.normal, 1e-2f)) return false;
		return true;
	}

	[[nodiscard]] Point3f shift(float epsilon) const {
		return position + normal * epsilon;
	}
};

using IntersectionList = std::vector<Intersection>;
} // namespace raytracing
#endif /* ifndef INTERSECTION_HPP */
