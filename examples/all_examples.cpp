#include "example.hpp"

namespace raytracing {
void run_all() {
	draw_chess_board();
	dodecahedron_overlap_test();
	multiple_sphere_test();
	torus_test("images/torus1.png", 0.0);
	torus_test("images/torus2.png", 0.7);
	polyhedra_test();
	kaleidoscope_test();
	bit_donut_test();
	saturn_test();
	block_test();
	cylinder_test();
	spheroid_test();
	cuboid_test();
	set_difference_test();
	set_intersection_test();
	sphere_test();
}
} // namespace raytracing
