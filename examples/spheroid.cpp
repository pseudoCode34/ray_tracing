#include "spheroid.hpp"

#include <cstddef>
#include <memory>
#include <utility>

#include "color.hpp"
#include "example.hpp"
#include "light_source.hpp"
#include "scene.hpp"
#include "solid_object.hpp"
#include "vector.hpp"

namespace raytracing::Imager {
void spheroid_test() {
	using namespace Imager;

	Scene scene;

	Spheroid spheroid(4.0, 2.0, 1.0);
	spheroid.move_point_to(0.0, 0.0, -50.0)
		.rotate(-12.0, 'x')
		.rotate(-60.0, 'y');

	scene.add_solid_object(std::make_unique<Spheroid>(std::move(spheroid)));
	scene.add_light_source(
		LightSource(Vector{+35.0, +50.0, +20.0}, Color(51, 255, 255)));
	scene.add_light_source(
		LightSource(Vector{-47.0, -37.0, +12.0}, Color(255, 51, 51)));

	const char *filename           = "images/spheroid.png";
	const size_t PIXEL_WIDTH       = 300;
	const size_t PIXEL_HEIGHT      = 300;
	const double ZOOM_FACTOR       = 8;
	const size_t ANTI_ALIAS_FACTOR = 2;

	scene.save_image(filename,
					 PIXEL_WIDTH,
					 PIXEL_HEIGHT,
					 ZOOM_FACTOR,
					 ANTI_ALIAS_FACTOR);
}
} // namespace raytracing::Imager
