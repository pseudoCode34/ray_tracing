#ifndef VOLUMETRIC_H
#define VOLUMETRIC_H

#include "spatial/primitive.hpp"

#include <optional>

namespace raytracing {

class Volumetric {
public:
	virtual ~Volumetric() = default;
	/**
	 * \brief Returns the intersection at minimal distance from \a ray if
	 * possible
	 */
	[[nodiscard]] virtual std::optional<float> hit(const Ray3D &ray) const = 0;

	/**
	 * \return True if this object contains the specified \a point
	 */
	[[nodiscard]] virtual bool contains(Vector3fConstRef other) const = 0;

	[[nodiscard]] virtual Vector3Df normal_at(Point3fConstRef point) const = 0;
	/*[>*
	 * \return True if this object contains any of the specified \a points
	 <]
	[[nodiscard]] virtual bool
	contains_any_of(std::span<Point3fConstRef> points) const
		= 0;
	[>*
	 * \return True if this object contains the specified volumetric object
	 <]
	[[nodiscard]] virtual bool contains(const Volumetric &other) const = 0;
	[>*
	 * \return The intersection of two volumetric entities.
	 <]
	[[nodiscard]] virtual std::optional<Volumetric>
	intersection(const Volumetric &other) const = 0;
	[>*
	 * \return The smallest volumetric entity that contains the two source
	 * entities.
	 <]
	[[nodiscard]] virtual Volumetric union_with(const Volumetric &other) const
		= 0;*/
};
} // namespace raytracing

#endif /* VOLUMETRIC_H */
