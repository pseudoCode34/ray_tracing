#include "spatial/viewport.hpp"

#include "quantity.hpp"
#include "spatial/camera.hpp"

#include <catch2/catch_test_macros.hpp>

namespace raytracing {
TEST_CASE("Viewport function test", "[viewport][camera]") {
	using mp_units::angular::unit_symbols::deg;
	Camera cam;
	cam.perspective(20.f * deg, 1.f, 0.1f, 10.f);
	Point3Df eye{13, 2, 3};
	cam.set_position(eye);
	cam.set_defocus_angle(0.6 * deg);
	cam.update_view_matrix();
	Viewport v = cam.set_viewport(1000 * pixel);
}
} // namespace raytracing
