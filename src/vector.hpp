#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "common_constants.hpp"

#include <fmt/format.h>

namespace raytracing::Imager {

struct Vector {
	double x, y, z;

	constexpr auto operator<=>(const Vector &) const = default;

	constexpr bool operator==(const Vector &other) const noexcept {
		if (abs(x - other.x) > EPSILON) return false;
		if (abs(y - other.y) > EPSILON) return false;
		if (abs(z - other.z) > EPSILON) return false;
		return true;
	}

	/*
	 * \brief Returns the square of the magnitude of this vector.
	 *
	 * \note This is much more efficient than computing the magnitude itself,
	 * and good for comparing two vectors to see which is longer or shorter.
	 */
	[[nodiscard]] constexpr double magnitude_squared() const {
		return x * x + y * y + z * z;
	}

	/*
	 * \brief Returns the magnitude of this vector.
	 * \note std::sqrt is not constexpr in C++20 yet! Only in C++23 or later.
	 * \see magnitude_squared()
	 */
	[[nodiscard]] constexpr double magnitude() const {
		return std::sqrt(magnitude_squared());
	}

	/*
	 * \brief Returns the square the magnitude of this vector.
	 *
	 * Treat this vector as 2D one, aka neglect the value of the Z-axis. Then
	 * compute its magnitude as normal.
	 *
	 * \see magnitude_squared()
	 */
	[[nodiscard]] constexpr double magnitude_sqr_in_plane() const {
		return x * x + y * y;
	}

	/**
	 * \brief Returns a unit vector, of this non-zero vector.
	 *
	 * In mathematics, a unit vector in a normed vector space is a vector (often
	 * a spatial vector) of length 1. The normalized vector û of a non-zero
	 * vector u is the unit vector in the direction of u.
	 *
	 * \return Vector A normalised vector
	 * \see https://en.wikipedia.org/wiki/Unit_vector
	 */
	[[nodiscard]] Vector unit_vector() const;

	/*
	 * \brief Returns the square the magnitude of this vector.
	 */
	[[nodiscard]] constexpr double dot(const Vector &other) const {
		return x * other.x + y * other.y + z * other.z;
	}

	/*
	 * \brief Returns the square the magnitude of this vector.
	 *
	 * Treat both vectors as 2D ones, aka neglect both value of the Z-axis.
	 * Then, multiply element-wise as normal.
	 *
	 * \return double The magnitude in Oxy plane
	 * \see dot_product()
	 */
	[[nodiscard]] constexpr double dot_in_plane(const Vector &other) const {
		return x * other.x + y * other.y;
	}

	/**
	 * \brief Returns a vector that is perpendicular to both origin vectors.
	 *
	 * Assume that \param lhs (denoted by a) and \param rhs (denoted by b) start
	 * at the origin point (0,0,0), the Cross Product will end at:
	 * \arg cx = aybz − azby
	 * \arg cy = azbx − axbz
	 * \arg cz = axby − aybx
	 *
	 * \return Vector A vector that is at right angles to both
	 * \see dot_product()
	 */
	[[nodiscard]] constexpr Vector cross(const Vector &other) const {
		return {y * other.z - z * other.y,
				z * other.x - x * other.z,
				x * other.y - y * other.x};
	}

	/*
	 * [Binary assymmetric operators]
	 * These operators overloading are member functions so only
	 * this->operator(other) is allowed, not opposite.
	 *
	 * If operator+ is a member function of the complex type, then only (complex
	 * + integer) would compile, and not (integer + complex)
	 */
	Vector &operator-=(const Vector &rhs);
	friend Vector operator-(Vector lhs, const Vector &rhs);

	Vector &operator/=(double scalar);
	friend Vector operator/(Vector vector, double scalar);

	Vector operator-() const;

	/*
	 * [vector.binary_symmetric_operators]
	 *
	 * Binary operators are typically implemented as non-members to maintain
	 * symmetry (for example, when adding a complex number and an integer, if
	 * operator+ is a member function of the complex type, then only complex +
	 * integer would compile, and not integer + complex)
	 */
	Vector &operator*=(double scalar);
	friend Vector operator*(Vector vector, double scalar);
	friend Vector operator*(double scalar, Vector vector);

	Vector &operator+=(const Vector &rhs);
	friend Vector operator+(Vector lhs, const Vector &rhs);
};
} // namespace raytracing::Imager


template <>
struct fmt::formatter<raytracing::Imager::Vector> {
	constexpr auto parse(fmt::format_parse_context &ctx) { return ctx.begin(); }

	constexpr auto format(const raytracing::Imager::Vector &vector,
						  fmt::format_context &ctx) {
		const auto [x, y, z] = vector;
		return fmt::format_to(ctx.out(), "({}, {}, {})", x, y, z);
	}
};
#endif /* ifndef VECTOR_HPP */
