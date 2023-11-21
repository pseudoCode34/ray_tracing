#include "sphere.hpp"

#include "color.hpp"
#include "example.hpp"
#include "light_source.hpp"
#include "scene.hpp"
#include "vector.hpp"

#include <memory>

namespace raytracing {
void multiple_sphere_test() {
	using namespace Imager;

	// Put spheres with different optical properties next to each other.
	Scene scene;
	Sphere shiny_sphere(Vector{-1.5, 0.0, -17.0}, 1.0);
	shiny_sphere.set_matte_gloss_balance(
		0.3,            // 30% of reflection is shiny, 70% is dull
		Color("white"), // matte color is white
		Color("white")  // gloss color is white
	);
	shiny_sphere.set_opacity(0.6);
	scene.add_solid_object(std::make_unique<Sphere>(std::move(shiny_sphere)));

	Sphere matte_sphere(Vector{+1.5, 0.0, -17.0}, 1.0);
	matte_sphere.set_full_matte(Color(153, 153, 230));
	matte_sphere.set_opacity(1.0);
	scene.add_solid_object(std::make_unique<Sphere>(std::move(matte_sphere)));

	scene.add_light_source(
		LightSource(Vector{-45.0, +10.0, +50.0}, Color("white")));
	scene.add_light_source(
		LightSource(Vector{+5.0, +90.0, -40.0}, Color("white")));
	scene.add_light_source(
		LightSource(Vector{+45.0, -10.0, +40.0}, Color("white")));


	const char *filename           = "images/multisphere.png";
	const size_t PIXEL_WIDTH       = 600;
	const size_t PIXEL_HEIGHT      = 360;
	const double ZOOM_FACTOR       = 4.5;
	const size_t ANTI_ALIAS_FACTOR = 1;
	scene.save_image(filename,
					 PIXEL_WIDTH,
					 PIXEL_HEIGHT,
					 ZOOM_FACTOR,
					 ANTI_ALIAS_FACTOR);
}

void sphere_test() {
	using namespace Imager;

	Scene scene;

	const Vector SPHERE_CENTER{0.0, 0.0, -40.0};
	const Vector LIGHT_DISPLACEMENT{-30.0, +120.0, +50.0};

	Sphere sphere(SPHERE_CENTER, 5.5);
	sphere.set_full_matte(Color(153, 51, 51));
	scene.add_solid_object(std::make_unique<Sphere>(std::move(sphere)));

	// Add a light source to illuminate the objects in the scene; otherwise we
	// won't see anything!
	scene.add_light_source(
		LightSource(SPHERE_CENTER + LIGHT_DISPLACEMENT, Color(128, 255, 77)));

	// Generate a PNG file that displays the scene...
	const char *filename           = "images/sphere.png";
	const size_t PIXEL_WIDTH       = 320;
	const size_t PIXEL_HEIGHT      = 400;
	const double ZOOM_FACTOR       = 3.0;
	const size_t ANTI_ALIAS_FACTOR = 1;

	scene.save_image(filename,
					 PIXEL_WIDTH,
					 PIXEL_HEIGHT,
					 ZOOM_FACTOR,
					 ANTI_ALIAS_FACTOR);
}
} // namespace raytracing
