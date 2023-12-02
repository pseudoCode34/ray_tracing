#include "torus.hpp"

#include "color.hpp"
#include "example.hpp"
#include "light_source.hpp"
#include "scene.hpp"
#include "set.hpp"

#include <memory>

namespace raytracing {
void torus_test(const char *image_file, double gloss_factor) {
	using namespace Imager;

	Scene scene;

	auto *torus1 = new Torus(3.0, 255);
	auto *torus2 = new Torus(3.0, 255);
	torus1->set_matte_gloss_balance(gloss_factor);
	torus1->move_point_to(+1.5, 0.0, 0.0);

	torus2->set_matte_gloss_balance(gloss_factor);
	torus2->move_point_to(-1.5, 0.0, 0.0).rotate(-90.0, 'x');

	auto *double_torus = new SetUnion(Vector{}, torus1, torus2);

	scene.add_solid_object(std::unique_ptr<SolidObject>(double_torus));

	double_torus->move_point_to(0.0, 0.0, -50.0);
	double_torus->rotate(-45.0, 'x').rotate(-30.0, 'y');

	// Add a light source to illuminate the objects in the scene; otherwise we
	// won't see anything!
	scene.add_light_source(
		LightSource(Vector{-45.0, +10.0, +50.0}, Color(255, 255, 77)));

	scene.add_light_source(
		LightSource(Vector{+5.0, +90.0, -40.0}, Color("grey", 0.5)));

	// Generate a PNG file that displays the scene...
	const size_t PIXEL_WIDTH       = 420;
	const size_t PIXEL_HEIGHT      = 300;
	const double ZOOM_FACTOR       = 5.5;
	const size_t ANTI_ALIAS_FACTOR = 2;
	scene.save_image(image_file,
					 PIXEL_WIDTH,
					 PIXEL_HEIGHT,
					 ZOOM_FACTOR,
					 ANTI_ALIAS_FACTOR);
}
} // namespace raytracing
