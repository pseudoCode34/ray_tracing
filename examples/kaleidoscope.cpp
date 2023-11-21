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

#include <array>
#include <math.h>
#include <memory>

namespace raytracing {
void add_kaleidoscope_mirrors(Imager::Scene &scene, double width,
							  double depth) {
	using namespace Imager;

	// Create three cuboids, one for each mirror
	double a = width * 2;
	double b = 0.1; // thickness doesn't really matter
	double c = depth * 2;

	Optics optics;
	optics.balance_matte_gloss(1.0, Color("white"), Color("white"));

	std::array<SolidObject *, 3> mirror;
	for (int i = 0; i < 3; ++i) {
		mirror[i] = new Cuboid(a, b, c);
		mirror[i]->set_uniform_optics(optics);

		scene.add_solid_object(std::unique_ptr<SolidObject>(mirror[i]));
	}

	// Rotate/translate the three mirrors differently.
	// Use the common value 's' that tells how far
	// the midpoints are from the origin.
	double s = b + a / sqrt(3.0);

	// Pre-calculate trig functions used more than once.
	double angle = radian_from_degree(30.0);
	double ca    = cos(angle);
	double sa    = sin(angle);

	// The bottom mirror just moves down (in the -y direction)
	mirror[0]->translate(0.0, -s, 0.0);

	// The upper left mirror moves up and left
	// and rotates +60 degrees around z axis.
	mirror[1]->translate(-s * ca, s * sa, 0.0);
	mirror[1]->rotate_z(+60.0);

	// The upper right mirror moves up and right
	// and rotates -60 degrees around z axis.
	mirror[2]->translate(s * ca, s * sa, 0.0);
	mirror[2]->rotate_z(-60.0);
}

void kaleidoscope_test() {
	using namespace Imager;

	Scene scene;
	add_kaleidoscope_mirrors(scene, 0.5, 10.0);

	Optics optics; // use default optics (white matte finish, opaque)

	auto *dodecahedron
		= new Dodecahedron(Vector{+0.0, 0.0, -50.0}, 1.0, optics);
	dodecahedron->rotate_x(-12.0);
	dodecahedron->rotate_y(-7.0);

	// Create a sphere that overlaps with the dodecahedron.
	const Vector SPHERE_CENTER{+255, 0.0, -50.0};
	auto *sphere = new Sphere(SPHERE_CENTER, 1.8);

	SetIntersection isect(SPHERE_CENTER, dodecahedron, sphere);

	isect.rotate_z(19.0);
	isect.translate(0.0, 0.0, 10.0);

	scene.add_solid_object(std::make_unique<SetIntersection>(std::move(isect)));

	scene.add_light_source(
		LightSource(Vector{-45.0, +10.0, +50.0}, Color(255, 255, 77)));
	scene.add_light_source(
		LightSource(Vector{+5.0, +90.0, -40.0}, Color("grey", 0.5)));
	scene.add_light_source(
		LightSource(Vector{+45.0, -10.0, +40.0}, Color(26, 128, 26, 0.5)));
	scene.add_light_source(
		LightSource(Vector{0.0, 0.0, +26}, Color(128, 26, 26)));

	/* scene.add_debug_point(419, 300); */
	/* scene.add_debug_point(420, 300); */
	/* scene.add_debug_point(421, 300); */

	const char *filename           = "images/kaleid.png";
	const size_t PIXEL_WIDTH       = 600;
	const size_t PIXEL_HEIGHT      = 600;
	const double ZOOM_FACTOR       = 6.0;
	const size_t ANTI_ALIAS_FACTOR = 1;

	scene.save_image(filename,
					 PIXEL_WIDTH,
					 PIXEL_HEIGHT,
					 ZOOM_FACTOR,
					 ANTI_ALIAS_FACTOR);
}
} // namespace raytracing
