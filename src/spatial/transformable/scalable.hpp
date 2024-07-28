#ifndef SCALABLE_H
#define SCALABLE_H

#include "spatial/primitive.hpp"

namespace raytracing {
class Scalable {
public:
	virtual ~Scalable() = default;

	/**
	 * Scales the object by \param vector
	 */
	virtual void scale_by(Vector3fConstRef vector) = 0;

	/**
	 * \return The object after being scale by \param vector
	 */
	//[[nodiscard]] virtual Scalable scaled_by(Vector3fConstRef vector) const =
	// 0;

	/**
	 * \return The object after being uniformly scaled by \param scalar.
	 */
	// virtual Scalable uniformly_scaled_by(float scalar) = 0;

	/**
	 * Uniformly scales the object by \param scalar
	 */
	virtual void uniformly_scale_by(float scalar) = 0;
};
} // namespace raytracing
#endif /* SCALABLE_H */
