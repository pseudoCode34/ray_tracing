#ifndef GEOMETRIC_HPP
#define GEOMETRIC_HPP
#include "color.hpp"

namespace raytracing {
/**
 * \brief Determine what fraction of the light is reflected at the
 * interface to calculate total reflection
 *
 * \param incident A normalised direction vector
 * \param normal A normalized plane normal vector
 * \param eta A ratio indicating whether \a incident is crossing into a less
 * dense medium if within (0,1), denser medium if greater than 1
 *
 * \returns A float in range [0,1)
 *
 * \note Regardless of whether the \a incident ray is outside or inside the
 * object, just consider the acute angle between the \a incidence and the \a
 * normal of the surface.
 *
 * \note Use Schlick's approximation.
 */
[[nodiscard]] float reflectance(Vector3fConstRef incident,
								Vector3fConstRef normal, float eta);

/**
 * \return A new color that is the total contribution of \a low light and \a
 * high one following energy preserving rule.
 *
 * \param t A ratio indicating how many percent light is low
 * \param low A color of the low light
 * \param high A color of the high light
 * FIXME: what if t is not in range of [0,1]?
 */
ScaledColor lerp(float t, const ScaledColor &low, const ScaledColor &high);

/**
 * \brief Computes the reflection of a vector based on a given normal using the
 * Householder transformation.
 *
 * This function calculates the reflection of an incident vector relative to a
 * surface normal. The Householder transformation is represented by the matrix:
 * \f[
 * H = I - 2 \cdot nn^T
 * \f]
 * where \f$ I \f$ is the identity matrix, \f$ n \f$ is the surface normal
 * vector, and \f$ n^T \f$ is its transpose. The transformation reflects a
 * vector across the plane defined by the normal.
 *
 * \param incident The incident vector that is being reflected.
 * \param normal The normal vector of the surface.
 *
 * \return Vector3f The reflected vector.
 *
 * \warning The normal vector is presumed to be normalized
 *
 * \sa glm::reflect()
 */
[[nodiscard]] Vector3f reflect(Vector3fConstRef incident,
							   Vector3fConstRef normal);

/**
 * \warning Presume normal and incident be normalised
 *
 * \note If \a incident is inside the object, incident.dot(normal)<0, negate \a
 * normal and inverse \a eta \sa glm::refract()
 *
 * \see glm::refract()
 */
Vector3f refract(Vector3fConstRef incident, Vector3fConstRef normal, float eta);
} // namespace raytracing
#endif /* ifndef GEOMETRIC_HPP */
