#ifndef ROTATABLE_HPP
#define ROTATABLE_HPP
#include <Eigen/Geometry>

namespace raytracing {
class Rotatable {
public:
	/**
	 * Rotates the object by a \a quaternion.
	 */
	virtual void rotate_by(const Eigen::Quaternionf &quaternion) = 0;

	/**
	 * \return The entity that a quaternion rotates.
	 */
	/*[[nodiscard]] virtual Rotatable
	rotated_by(const Eigen::Quaternionf &quaternion) const
		= 0;*/
	virtual ~Rotatable() = default;
};
} // namespace raytracing

#endif /* ROTATABLE_HPP */
