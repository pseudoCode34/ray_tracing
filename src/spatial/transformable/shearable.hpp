#ifndef SHEARABLE_HPP
#define SHEARABLE_HPP

#include "spatial/primitive.hpp"

namespace raytracing {
class Shearable {
public:
	virtual ~Shearable() = default;

	/**
	 * Sheares the object by \param vector
	 */
	virtual void scale_by(Vector3fConstRef vector) = 0;

	/**
	 * \return The object after being scale by \param vector
	 */
	[[nodiscard]] virtual Shearable scaled_by(Vector3fConstRef vector) const
		= 0;

	/**
	 * \return The object after being uniformly scaled by \param scalar.
	 */
	virtual Shearable uniformly_scaled_by(float scalar) = 0;

	/**
	 * Uniformly scales the object by \param scalar
	 */
	virtual void uniformly_scale_by(float scalar) = 0;
};
} // namespace raytracing

#endif /* ifndef SHEARABLE_HPP */
