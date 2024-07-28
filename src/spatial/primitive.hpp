#ifndef PRIMITIVE_HPP
#define PRIMITIVE_HPP
#include <Eigen/Geometry>

namespace raytracing {
using Vector3Df        = Eigen::Vector3f;
using Vector3fConstRef = const Eigen::Ref<const Eigen::Vector3f>;
using Vector3fRef      = Eigen::Ref<Eigen::Vector3f>;

using Point3Df        = Eigen::Vector3f;
using Point3fConstRef = const Eigen::Ref<const Point3Df>;
using Point3fRef      = Eigen::Ref<Point3Df>;

using ScaledColor         = Eigen::Vector3f;
using ScaledColorConstRef = const Eigen::Ref<const ScaledColor>;
using ScaledColorRef      = Eigen::Ref<ScaledColor>;

using Ray3D = Eigen::ParametrizedLine<float, 3>;

} // namespace raytracing


#endif /* PRIMITIVE_HPP */
