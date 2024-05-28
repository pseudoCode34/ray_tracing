#ifndef POINT3D_HPP
#define POINT3D_HPP

#include "vector3f.hpp"

namespace raytracing {
using Point3f         = Vector3f;
using Point3fConstRef = const Eigen::Ref<const Point3f>;
using Point3fRef      = Eigen::Ref<Point3f>;
} // namespace raytracing
#endif /* ifndef POINT3D_HPP */
