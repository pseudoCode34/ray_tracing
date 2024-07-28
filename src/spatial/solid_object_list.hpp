#ifndef SOLID_OBJECT_LIST_HPP
#define SOLID_OBJECT_LIST_HPP

#include "intersection.hpp"
#include "owning_container.hpp"
#include "transformable/solid_object.hpp"

#include <algorithm>
#include <optional>

namespace raytracing {

class [[nodiscard]] SolidObjectList : public OwningContainer<SolidObject> {
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
	find_closest_intersection(const Ray3D &ray) const {
		std::vector<RayRootInfo> list;
		for (const auto &solid : container())
			if (const auto root = solid->hit(ray); root.has_value())
				list.emplace_back(solid.get(), root.value());
		if (list.empty()) return std::nullopt;

		const auto &[solid, root]
			= std::ranges::min(list, {}, &RayRootInfo::root);

		const Point3Df position = ray.pointAt(root);
		const Vector3Df normal  = solid->normal_at(position);

		return std::make_optional<Intersection>(position,
												normal,
												&solid->get_optics());
	}

	[[nodiscard]] constexpr std::optional<element_type *>
	find_any_primary_container(const Point3Df &point) const {
		return find_any(
			[&point](const auto &solid) { return solid->contains(point); });
	}
};
} // namespace raytracing
#endif /* ifndef SOLID_OBJECT_LIST_HPP */
