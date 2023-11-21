#include "block.hpp"

#include "color.hpp"
#include "example.hpp"
#include "light_source.hpp"
#include "optics.hpp"
#include "scene.hpp"
#include "sphere.hpp"
#include "vector.hpp"

#include <memory>

namespace raytracing {
void block_test() {
	using namespace Imager;

	// We use a "Scene" object to embody the entire model
	Scene scene(Color(94, 115, 94, 7.0e-6));

	// Create a concrete block object.
	// We *must* use operator new to create all solids, because the Scene
	// destructor will delete it.
	ConcreteBlock block(Vector{0.0, 0.0, -155.0}, Optics(Color("grey")));
	block.rotate_x(-10.0).rotate_y(-15.0);
	scene.add_solid_object(std::make_unique<ConcreteBlock>(std::move(block)));

	// Create a sphere and put it into the scene also.
	Sphere sphere(Vector{10.0, 3.0, -147.0}, 3.5);
	sphere.set_full_matte(Color(153, 102, 115));
	scene.add_solid_object(std::make_unique<Sphere>(std::move(sphere)));

	// Add a light source to illuminate the objects in the scene; otherwise we
	// won't see anything!
	scene.add_light_source(
		LightSource(Vector{+20.0, +20.0, +80.0}, Color(128, 26, 26, 265)));
	scene.add_light_source(
		LightSource(Vector{+100.0, +120.0, -70.0}, Color(51, 128, 102)));
	scene.add_light_source(
		LightSource(Vector{+3.0, +13.0, +80.0}, Color(153, 128, 77, 1.20)));

	// Generate a PNG file that displays the scene...
	const char *filename           = "images/block.png";
	const size_t PIXEL_WIDTH       = 320;
	const size_t PIXEL_HEIGHT      = 400;
	const double ZOOM_FACTOR       = 4.5;
	const size_t ANTI_ALIAS_FACTOR = 4;

	scene.save_image(filename,
					 PIXEL_WIDTH,
					 PIXEL_HEIGHT,
					 ZOOM_FACTOR,
					 ANTI_ALIAS_FACTOR);
}
} // namespace raytracing
