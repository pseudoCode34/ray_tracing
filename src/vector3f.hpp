#ifndef VECTOR_HPP
#define VECTOR_HPP

// IWYU pragma: private, include "point3f.h"
#include <Eigen/Core> // IWYU pragma: keep

namespace raytracing {
using Vector3f         = Eigen::Vector3f;
using Vector3fConstRef = const Eigen::Ref<const Eigen::Vector3f>;
using Vector3fRef      = Eigen::Ref<Eigen::Vector3f>;

/**
 * \brief Determines if the \a incident light is hitting the surface from outer
 *
 * \param normal The normalised vector of the surface at the hitting point is
 * presumed to point outward
 * \param incident A vector hitting the surface
 *
 * \return The cosine of the angle of those 2 vectors, which is the dot product,
 * if positive. If it is negative (the \a incident light hitting from inside) or
 * zero (the \a light parallel to the \normal), return std::nullopt
 */
float is_facing_forward(Vector3fConstRef normal, Vector3fConstRef incident);

} // namespace raytracing

#endif /* ifndef VECTOR_HPP */
