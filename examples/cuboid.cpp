#include <memory>
#include <string_view>
#include <utility>

#include "color.hpp"
#include "example.hpp"
#include "light_source.hpp"
#include "polyhedra.hpp"
#include "scene.hpp"
#include "vector.hpp"

namespace raytracing::Imager {
void cuboid_test() {
	using namespace Imager;

	Scene scene;

	Cuboid cuboid(3.0, 4.0, 2.0);
	cuboid.set_full_matte(Color(255, 51, 128));
	cuboid.move_point_to(0.0, 0.0, -50.0);
	cuboid.rotate(-115.0, 'x');
	cuboid.rotate(-22.0, 'y');

	scene.add_solid_object(std::make_unique<Cuboid>(std::move(cuboid)));
	scene.add_light_source(
		LightSource(Vector{-5.0, 50.0, +20.0}, Color("white")));

	const char *filename           = "images/cuboid.png";
	const size_t PIXEL_WIDTH       = 300;
	const size_t PIXEL_HEIGHT      = 300;
	const double ZOOM_FACTOR       = 3.0;
	const size_t ANTI_ALIAS_FACTOR = 2;

	scene.save_image(filename,
					 PIXEL_WIDTH,
					 PIXEL_HEIGHT,
					 ZOOM_FACTOR,
					 ANTI_ALIAS_FACTOR);
}
} // namespace raytracing::Imager
