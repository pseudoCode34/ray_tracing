#include "color.hpp"
#include "example.hpp"
#include "light_source.hpp"
#include "scene.hpp"
#include "set.hpp"
#include "solid_object.hpp"
#include "sphere.hpp"
#include "vector.hpp"

#include <memory>

namespace raytracing {
void set_intersection_test() {
	using namespace Imager;

	Scene scene;

	// Display the intersection of two overlapping spheres.
	const double RADIUS = 1.0;

	SolidObject *sphere1 = new Sphere(Vector{-0.5, 0.0, 0.0}, RADIUS),
				*sphere2 = new Sphere(Vector{-0.5, 0.0, 0.0}, RADIUS);
	sphere1->set_full_matte(Color(255, 255, 128));
	sphere2->set_full_matte(Color(255, 128, 255));

	SolidObject *isect = new SetIntersection(Vector{}, sphere1, sphere2);

	isect->move_point_to(0.0, 0.0, -50.0);
	isect->rotate_y(-12.0);
	isect->rotate_x(-28.0);

	scene.add_solid_object(std::unique_ptr<SolidObject>(isect));
	scene.add_light_source(
		LightSource(Vector{-5.0, 10.0, +80.0}, Color("white")));

	const char *filename           = "images/set_intersection.png";
	const size_t PIXEL_WIDTH       = 300;
	const size_t PIXEL_HEIGHT      = 300;
	const double ZOOM_FACTOR       = 25.0;
	const size_t ANTI_ALIAS_FACTOR = 2;

	scene.save_image(filename,
					 PIXEL_WIDTH,
					 PIXEL_HEIGHT,
					 ZOOM_FACTOR,
					 ANTI_ALIAS_FACTOR);
}
} // namespace raytracing
