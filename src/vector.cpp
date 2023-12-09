#include "vector.hpp"

namespace raytracing {
namespace Imager {

Vector Vector::unit_vector() const {
	const double MAG = magnitude();
	return *this / MAG;
}

Vector &Vector::operator*=(double scalar) {
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}

Vector &Vector::operator+=(const Vector &rhs) {
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
}

Vector &Vector::operator-=(const Vector &rhs) {
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	return *this;
}

Vector operator-(Vector lhs, const Vector &rhs) { return lhs -= rhs; }

Vector &Vector::operator/=(double scalar) {
	x /= scalar;
	y /= scalar;
	z /= scalar;
	return *this;
}

Vector operator/(Vector vector, double scalar) { return vector /= scalar; }

Vector operator*(Vector vector, double scalar) { return vector *= scalar; }

Vector operator*(double scalar, Vector vector) { return vector *= scalar; }

//  passing lhs by value helps optimize chained a+b+c:
Vector operator+(Vector lhs, const Vector &rhs) { return lhs += rhs; }

Vector Vector::operator-() const { return Vector{-x, -y, -z}; }

} // namespace Imager
} // namespace raytracing
