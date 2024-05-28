#include "camera.hpp"
#include "color.hpp"
#include "example.hpp"
#include "material.hpp"
#include "quantity.hpp"
#include "random.hpp"
#include "rect.hpp"
#include "sphere.hpp"
#include "viewport.hpp"

#include <memory>
#include <mp-units/systems/angular/angular.h>

namespace raytracing {

void draw_sphere() {
	using mp_units::angular::unit_symbols::deg;
	Rect dimension{1000, 1000};

	Camera cam;
	cam.perspective(20.f * deg, dimension.aspect_ratio(), 0.1f, 10.f);
	cam.set_position(Point3f{13, 2, 3});
	cam.set_defocus_angle(0.6 * deg);
	cam.update_view_matrix();

	ImageRenderer r{cam.orig(), 1, 5};

	Viewport vp = cam.set_viewport(dimension);
	r.add(
		std::make_unique<Sphere>(Point3f{0, -100, 0},
								 Material{ScaledColor(0, 0.3, 0.5), 1, 0, 1.3},
								 100));
	for (int a = 1; a < 6; a++) {
		r.add(std::make_unique<Sphere>(
			random_vector(-a, a),
			Material{ScaledColor{0.2, 0.3, 0.4}, 0.2, 0.7, 1.4},
			0.2));
	}

	r.add(std::make_unique<Sphere>(
		Point3f{-4, 1, 0},
		Material{ScaledColor(1, 0, 0.2), 0.5, 0.7, 1.8},
		1.0));
	r.add(std::make_unique<Sphere>(
		Point3f{5, 1, 0},
		Material{ScaledColor(0.5, 0.6, 0.8), 0.3, 0.65, 1.6},
		1.0));

	/* r.set_light_sources({LightSource{Point3f{0, 5, 0}, ScaledColor{1, 0,
	 * 0}}}); */
	r.save_image("sphere.ppm", vp, dimension);
}
} // namespace raytracing
