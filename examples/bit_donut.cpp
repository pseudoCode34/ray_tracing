#include "color.hpp"
#include "example.hpp"
#include "light_source.hpp"
#include "scene.hpp"
#include "set.hpp"
#include "solid_object.hpp"
#include "sphere.hpp"
#include "torus.hpp"
#include "vector.hpp"

#include <memory>
#include <stddef.h>

namespace raytracing {
void bit_donut_test() {
	using namespace Imager;

	Scene scene;

	auto *torus     = new Torus(3.0, 1.0);
	auto *sphere    = new Sphere(Vector{-3.0, 0.0, 0.0}, 1.5);
	auto *bit_donut = new SetDifference(Vector{}, torus, sphere);

	// This line shows that the object added in scene need to move and rotate.
	// Is it clear enough that we wont misunderstood the whole scene "move"?
	scene.add_solid_object(std::unique_ptr<SolidObject>(bit_donut))
		->move_point_to(0.0, 0.0, -50.0)
		.rotate(-45.0, 'x')
		.rotate(-30.0, 'y');

	// Add a light source to illuminate the objects in the scene; otherwise we
	// won't see anything!
	scene.add_light_source(
		LightSource(Vector{-45.0, +10.0, +50.0}, Color(255, 255, 77)));
	scene.add_light_source(
		LightSource(Vector{+5.0, +90.0, -40.0}, Color("grey", 0.5)));

	const char *filename           = "images/donutbite.png";
	const size_t PIXEL_WIDTH       = 300;
	const size_t PIXEL_HEIGHT      = 300;
	const double ZOOM_FACTOR       = 6.0;
	const size_t ANTI_ALIAS_FACTOR = 2;
	scene.save_image(filename,
					 PIXEL_WIDTH,
					 PIXEL_HEIGHT,
					 ZOOM_FACTOR,
					 ANTI_ALIAS_FACTOR);
}
} // namespace raytracing
