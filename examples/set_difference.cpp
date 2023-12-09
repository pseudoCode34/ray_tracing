#include <cstddef>
#include <memory>
#include <string_view>
#include <utility>

#include "color.hpp"
#include "example.hpp"
#include "light_source.hpp"
#include "scene.hpp"
#include "set.hpp"
#include "solid_object.hpp"
#include "sphere.hpp"
#include "vector.hpp"

namespace raytracing::Imager {
void set_difference_test() {
	using namespace Imager;

	Scene scene;

	const Color SPHERE_COLOR(255, 255, 128);

	// Display the intersection of two overlapping spheres.
	auto make_sphere = [&SPHERE_COLOR](Vector &&location, double radius) {
		SolidObject *sphere1 = new Sphere(std::move(location), radius);
		sphere1->set_full_matte(SPHERE_COLOR);
		return sphere1;
	};

	SolidObject *diff
		= new SetIntersection(Vector{},
							  make_sphere(Vector{-0.5, 0.0, 0.0}, 1.0),
							  make_sphere(Vector{0.5, 0.0, 0.0}, 1.3));
	diff->move_point_to(1.0, 0.0, -50.0);
	diff->rotate(-5.0, 'y');

	scene.add_solid_object(std::unique_ptr<SolidObject>(std::move(diff)));

	scene.add_light_source(
		LightSource(Vector{-5.0, 50.0, +20.0}, Color("white")));

	const char *filename           = "images/set_difference.png";
	const size_t PIXEL_WIDTH       = 300;
	const size_t PIXEL_HEIGHT      = 300;
	const double ZOOM_FACTOR       = 20.0;
	const size_t ANTI_ALIAS_FACTOR = 2;

	scene.save_image(filename,
					 PIXEL_WIDTH,
					 PIXEL_HEIGHT,
					 ZOOM_FACTOR,
					 ANTI_ALIAS_FACTOR);
}
} // namespace raytracing::Imager
