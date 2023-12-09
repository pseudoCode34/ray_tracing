#include "block.hpp"

#include <cstddef>
#include <memory>
#include <string_view>
#include <utility>

#include "color.hpp"
#include "example.hpp"
#include "light_source.hpp"
#include "optics.hpp"
#include "scene.hpp"
#include "solid_object.hpp"
#include "sphere.hpp"
#include "vector.hpp"

namespace raytracing::Imager {
void block_test() {
	using namespace Imager;

	Scene scene(Color(94, 115, 94));

	ConcreteBlock block(Vector{0.0, 0.0, -155}, Optics(Color("grey")));
	block.rotate(-10.0, 'x').rotate(-15.0, 'y');
	scene.add_solid_object(std::make_unique<ConcreteBlock>(block));

	// Create a sphere and put it into the scene also.
	Sphere sphere(Vector{10., 3., -147}, 3.5);
	sphere.set_full_matte(Color(153, 102, 115));
	scene.add_solid_object(std::make_unique<Sphere>(std::move(sphere)));

	// Add a light source to illuminate the objects in the scene; otherwise we
	// won't see anything!
	scene.add_light_source(
		LightSource(Vector{+20.0, +20.0, +80.0}, Color(128, 26, 26)));
	scene.add_light_source(
		LightSource(Vector{+100.0, +120.0, -70.0}, Color(51, 128, 102)));
	scene.add_light_source(
		LightSource(Vector{+3.0, +13.0, +80.0}, Color(153, 128, 77)));

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
} // namespace raytracing::Imager
