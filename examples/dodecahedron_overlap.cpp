#include <cstddef>
#include <memory>
#include <string_view>

#include "color.hpp"
#include "example.hpp"
#include "light_source.hpp"
#include "optics.hpp"
#include "polyhedra.hpp"
#include "scene.hpp"
#include "set.hpp"
#include "solid_object.hpp"
#include "sphere.hpp"
#include "vector.hpp"

namespace raytracing::Imager {
void dodecahedron_overlap_test() {
	using namespace Imager;

	Scene scene;
	Optics optics; // use default optics (white matte finish, opaque)

	auto *dodecahedron
		= new Dodecahedron(Vector{+0.0, 0.0, -50.0}, 1.0, optics);
	dodecahedron->rotate(-12.0, 'x').rotate(-7.0, 'y');

	// Create a sphere that overlaps with the dodecahedron.
	const Vector SPHERE_CENTER{+255, 0.0, -50.0};
	auto *sphere = new Sphere(SPHERE_CENTER, 1.8);
	auto *isect  = new SetIntersection(SPHERE_CENTER, dodecahedron, sphere);

	scene.add_solid_object(std::unique_ptr<SetIntersection>(isect));

	scene.add_light_source(
		LightSource(Vector{-45.0, +10.0, +50.0}, Color(255, 255, 77)));
	scene.add_light_source(
		LightSource(Vector{+5.0, +90.0, -40.0}, Color("grey", 0.5)));
	scene.add_light_source(
		LightSource(Vector{+45.0, -10.0, +40.0}, Color(26, 128, 26, 128)));

	const char *filename           = "images/overlap.png";
	const size_t PIXEL_WIDTH       = 400;
	const size_t PIXEL_HEIGHT      = 300;
	const double ZOOM_FACTOR       = 12.0;
	const size_t ANTI_ALIAS_FACTOR = 1;

	scene.save_image(filename,
					 PIXEL_WIDTH,
					 PIXEL_HEIGHT,
					 ZOOM_FACTOR,
					 ANTI_ALIAS_FACTOR);
}
} // namespace raytracing::Imager
