#ifndef RAY_HPP
#define RAY_HPP
#include "point3f.hpp"
#include "vector3f.hpp"

namespace raytracing {
struct Ray {
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	Point3f origin     = Point3f::Zero();
	Vector3f direction = -Vector3f::UnitZ();

	[[nodiscard]] Point3f at(float t) const;

	[[nodiscard]] bool contains(Point3fConstRef point) const;
};

/**
 * \brief Returns a ray cutting through 2 given points
 *
 * \param from The starting point
 * \param to The point passed through
 * \return A new ray starting from point \a from and looking in the direction of
 * \a to
 *
 * \warning The direction is not normalized yet, because when calls
 * SolidObject::hit(), the equation needs the direction.squaredNorm() to get
 * distance, So if normalized() before, it will return 1, which is incorrect.
 * So, be cautious when use with trace_ray()
 * \sa trace_ray()
 */
[[nodiscard]] Ray standard_form_of(Point3fConstRef from, Point3fConstRef to,
								   bool need_normalize = true);

} // namespace raytracing

#endif /* ifndef RAY_HPP */
