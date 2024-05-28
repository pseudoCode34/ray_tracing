#ifndef SOLID_OBJECT_LIST_HPP
#define SOLID_OBJECT_LIST_HPP

#include "intersection.hpp"
#include "owning_container.hpp"
#include "solid_object.hpp"

#include <algorithm>
#include <cmath>
#include <optional>
#include <vector>

namespace raytracing {

class SolidObjectList : public OwningContainer<SolidObject> {
	struct RayRootInfo {
		element_type *solid;
		float root;
	};

public:
	/**
	 * \brief Returns the intersection with any solid in the scene closest
	 * to the \a ray
	 */
	[[nodiscard]] std::optional<Intersection>
	find_closest_intersection(const Ray &ray) const {
		std::vector<RayRootInfo> list;
		for (const auto &solid : container())
			list.emplace_back(solid.get(), solid->hit(ray));
		if (list.empty()) return std::nullopt;

		const auto &[solid, root]
			= std::ranges::min(list, {}, &RayRootInfo::root);

		if (std::isinf(root)) return std::nullopt;
		const Point3f position = ray.at(root);
		const Vector3f normal  = solid->normal_at(position);

		return std::make_optional<Intersection>(position,
												normal,
												&solid->get_optics());
	}

	[[nodiscard]] constexpr std::optional<element_type *>
	find_any_primary_container(const Point3f &point) const {
		return find_any(
			[&point](const auto &solid) { return solid->contains(point); });
	}
};
} // namespace raytracing
#endif /* ifndef SOLID_OBJECT_LIST_HPP */
