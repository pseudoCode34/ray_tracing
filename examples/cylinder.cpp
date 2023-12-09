#include "cylinder.hpp"

#include <cstddef>
#include <memory>
#include <string_view>

#include "color.hpp"
#include "example.hpp"
#include "light_source.hpp"
#include "optics.hpp"
#include "planet.hpp"
#include "polyhedra.hpp"
#include "scene.hpp"
#include "solid_object.hpp"
#include "vector.hpp"

namespace raytracing::Imager {
void cylinder_test() {
	using namespace Imager;

	Scene scene;

	auto *cylinder = new Cylinder(2.0, 5.0);
	cylinder->set_full_matte(Color(255, 128, 128));
	cylinder->move_point_to(0.0, 0.0, -50.0)
		.rotate(-72.0, 'x')
		.rotate(-12.0, 'y');

	scene.add_solid_object(std::unique_ptr<SolidObject>(cylinder));
	scene.add_light_source(
		LightSource(Vector{+35.0, +50.0, +20.0}, Color("white")));

	const char *filename           = "images/cylinder.png";
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

void saturn_test() {
	using namespace Imager;

	Scene scene;
	auto *saturn = new Saturn();
	saturn->move_point_to(0.0, 0.0, -255.0);
	saturn->rotate(-15.0, 'y').rotate(-83.0, 'x');
	scene.add_solid_object(std::unique_ptr<SolidObject>(saturn));
	scene.add_light_source(
		LightSource(Vector{+30.0, +26.0, +20.0}, Color("white")));

	const char *filename           = "images/saturn.png";
	const size_t PIXEL_WIDTH       = 500;
	const size_t PIXEL_HEIGHT      = 250;
	const double ZOOM_FACTOR       = 4.0;
	const size_t ANTI_ALIAS_FACTOR = 4;

	scene.save_image(filename,
					 PIXEL_WIDTH,
					 PIXEL_HEIGHT,
					 ZOOM_FACTOR,
					 ANTI_ALIAS_FACTOR);
}

void polyhedra_test() {
	using namespace Imager;

	Scene scene;
	Optics optics; // use default optics (white matte finish, opaque)

	auto *icosahedron = new Icosahedron(Vector{-2.0, 0.0, -50.0}, 1.0, optics);
	icosahedron->rotate(-12.0, 'y').rotate(-7.0, 'x');
	scene.add_solid_object(std::unique_ptr<SolidObject>(icosahedron));

	auto *dodecahedron
		= new Dodecahedron(Vector{+2.0, 0.0, -50.0}, 1.0, optics);
	dodecahedron->rotate(-12.0, 'x').rotate(-7.0, 'y');
	scene.add_solid_object(std::unique_ptr<SolidObject>(dodecahedron));

	scene.add_light_source(
		LightSource(Vector{-45.0, +10.0, +50.0}, Color(255, 255, 77)));
	// FIXME green = 1.5?
	scene.add_light_source(
		LightSource(Vector{+5.0, +90.0, -40.0}, Color("grey", 0.5)));
	scene.add_light_source(
		LightSource(Vector{+45.0, -10.0, +40.0}, Color(26, 128, 26, 0.5)));

	const char *filename           = "images/polyhedra.png";
	const size_t PIXEL_WIDTH       = 600;
	const size_t PIXEL_HEIGHT      = 300;
	const double ZOOM_FACTOR       = 12.0;
	const size_t ANTI_ALIAS_FACTOR = 4;

	scene.save_image(filename,
					 PIXEL_WIDTH,
					 PIXEL_HEIGHT,
					 ZOOM_FACTOR,
					 ANTI_ALIAS_FACTOR);
}
} // namespace raytracing::Imager
