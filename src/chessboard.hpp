#ifndef __DDC_IMAGER_CHESSBOARD_H
#define __DDC_IMAGER_CHESSBOARD_H

#include "color.hpp"
#include "optics.hpp"
#include "polyhedra.hpp"

namespace raytracing {
namespace Imager {
struct Vector;

class ChessBoard : public Cuboid {
public:
	explicit ChessBoard(double inner_size, double x_border_size,
						double y_border_size, double thickness,
						const Color &light_square_color,
						const Color &dark_square_color,
						const Color &border_color);
	std::pair<int, int> chess_coordinate(const Vector &surface_point) const;

protected:
	// This method override provides the variety of colors for the light
	// squares, dark squares, and border.
	Optics object_space_surface_optics(const Vector &surface_point,
									   const void *context) const override;

	int square_coordinate(double xy) const;

private:
	double inner_size_;
	double x_border_size_;
	double y_border_size_;
	double thickness_;
	Color light_square_color_;
	Color dark_square_color_;
	Color border_color_;
};
} // namespace Imager

} // namespace raytracing
#endif // __DDC_IMAGER_CHESSBOARD_H
