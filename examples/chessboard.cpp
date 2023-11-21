#include "chessboard.hpp"

#include "example.hpp"
#include "light_source.hpp"
#include "scene.hpp"
#include "solid_object.hpp"
#include "sphere.hpp"
#include "vector.hpp"

#include <array>
#include <memory>

namespace raytracing {
struct sphere_info {
	Imager::Vector center;
	double radius;
};

void draw_chess_board() {
	using namespace Imager;

	Scene scene;

	// Dimensions of the chess board.

	const double INNER_SIZE    = 4.00;
	const double X_BORDER_SIZE = 1.00;
	const double Y_BORDER_SIZE = 1.00;
	const double THICKNESS     = 0.25;

	const Color LIGHT_SQUARE_COLOR = Color(192, 192, 192);
	const Color DARK_SQUARE_COLOR  = Color(112, 128, 144);
	const Color BORDER_COLOR       = Color("grey");

	ChessBoard board(INNER_SIZE,
					 X_BORDER_SIZE,
					 Y_BORDER_SIZE,
					 THICKNESS,
					 LIGHT_SQUARE_COLOR,
					 DARK_SQUARE_COLOR,
					 BORDER_COLOR);

	board.move_point_to(-0.35, 0.1, -20.0).rotate_z(+11.0).rotate_x(-62.0);

	scene.add_solid_object(std::make_unique<ChessBoard>(std::move(board)));


	/* 	{{Vector{0.0, 0.0, -17.0}, 0.72}, {Vector{+1.8, 0.0, -17.0}, 0.72}}); */

	/* for (const auto &[center, radius] : SINFO) { */
	/* 	Sphere sphere(center, radius); */
	/* 	sphere.set_opacity(0.6); */
	/* 	sphere.set_matte_gloss_balance(0.3, */
	/* 								   Color(0.6, 0.2, 0.2), */
	/* 								   Color(1, 1, 1)); */
	/* 	scene.add_solid_object(std::make_unique<Sphere>(std::move(sphere))); */
	/* } */

	scene.add_light_source(
		LightSource(Vector{-45.0, +25.0, +50.0}, Color(0, 63, 128)));
	scene.add_light_source(
		LightSource(Vector{+5.0, +90.0, +40.0}, Color(63, 63, 63)));
	/* scene.add_light_source(LightSource(Vector{}, Color(1, 1, 1))); */

	const char *filename           = "images/chessboard.png";
	const size_t PIXEL_WIDTH       = 600;
	const size_t PIXEL_HEIGHT      = 360;
	const double ZOOM_FACTOR       = 4.5;
	const size_t ANTI_ALIAS_FACTOR = 3;
	scene.save_image(filename,
					 PIXEL_WIDTH,
					 PIXEL_HEIGHT,
					 ZOOM_FACTOR,
					 ANTI_ALIAS_FACTOR);
}
} // namespace raytracing
