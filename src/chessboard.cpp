#include "chessboard.hpp"

#include "imager.hpp"
#include "vector.hpp"

#include <math.h>
#include <utility>

namespace raytracing::Imager {
bool is_inside_chechered_part(const Imager::Vector &surface_point,
							  double inner_size) {
	const double HALF = inner_size / 2.0;
	return fabs(surface_point.x) < HALF && fabs(surface_point.y) < HALF;
}

ChessBoard::ChessBoard(double inner_size, double x_border_size,
					   double y_border_size, double thickness,
					   const Color &light_square_color,
					   const Color &dark_square_color,
					   const Color &border_color)
	: Cuboid(inner_size / 2 + x_border_size, inner_size / 2 + y_border_size,
			 thickness / 2),
	  inner_size_(inner_size),
	  x_border_size_(x_border_size),
	  y_border_size_(y_border_size),
	  thickness_(thickness),
	  light_square_color_(light_square_color),
	  dark_square_color_(dark_square_color),
	  border_color_(border_color) {}

std::pair<int, int>
ChessBoard::chess_coordinate(const Vector &surface_point) const {
	return std::make_pair(square_coordinate(surface_point.x),
						  square_coordinate(surface_point.y));
}

int ChessBoard::square_coordinate(double xy) const {
	double s = floor(8.0 * (xy / inner_size_ + 0.5));
	if (s < 0.0) return 0;
	if (s > 7.0) return 7;
	return static_cast<int>(s);
}

Optics ChessBoard::object_space_surface_optics(const Vector &surface_point,
											   const void *context) const {
	// Start with the uniform optics this class inherits, and
	// modify the colors as needed. This allows us to inherit
	// gloss, refraction, etc.
	Optics optics = get_uniform_optics();

	// First figure out which part of the board this is. If the
	// t-coordinate (z in class Vector) is significantly below
	// the top surface, use the border color.
	if (surface_point.z < thickness_ / 2.0 - EPSILON)
		optics.set_matte_color(border_color_);
	else if (is_inside_chechered_part(surface_point, inner_size_)) {
		// Assume this is on the top surface of the board.
		// We are definitely inside the checkered part of the top surface.
		// Figure out which square we are on, so we can in turn figure out
		// whether it is a light or dark square, so we know how to color it.
		const auto [x, y] = chess_coordinate(surface_point);
		if ((x + y) & 1) optics.set_matte_color(light_square_color_);
		else optics.set_matte_color(dark_square_color_);
	} else
		// Outside the checkered part, so use border color.
		optics.set_matte_color(border_color_);

	return optics;
}
} // namespace raytracing::Imager
