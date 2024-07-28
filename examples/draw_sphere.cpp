#include "example.hpp"
#include "graphics/image.hpp"
#include "quantity/pixel.hpp"
#include "spatial/camera.hpp"
#include "spatial/image_renderer.hpp"
#include "spatial/material.hpp"
#include "spatial/primitive.hpp"
#include "spatial/refractive_index.hpp"
#include "spatial/shapes/sphere.hpp"

#include <filesystem>
#include <memory>
#include <mp-units/systems/angular/angular.h>
#include <mp-units/systems/si/unit_symbols.h>
#include <spdlog/spdlog.h>

namespace raytracing {

void draw_sphere() {
	using mp_units::angular::unit_symbols::deg;
	using mp_units::si::unit_symbols::m;
	Eigen::initParallel();

	Camera cam;
	cam.perspective(20.f * deg, 1920.f / 1080, 0.1f, 10.f);
	cam.set_position(Point3Df{13, 2, 3});
	cam.set_defocus_angle(0.6 * deg);
	cam.update_view_matrix();

	ImageRenderer scene{cam.set_viewport(1080 * pixel),
						ScaledColor::Ones(),
						RefractiveIndex(1),
						5};

	scene.add(std::make_unique<Sphere>(Point3Df{0, -100, 0},
									   Material{ScaledColor(0, 0.3, 0.5),
												ScaledColor(0.3, 0.9, 0.4),
												ScaledColor(0, 0.1, 0.2),
												1,
												0,
												RefractiveIndex(1.3)},
									   100.f * m));

	scene.add(std::make_unique<Sphere>(Point3Df{-4, 1, 0},
									   Material{ScaledColor(1, 0, 0.2),
												ScaledColor(0, 0.3, 0.5),
												ScaledColor(0.9, 0.5, 1),
												0.5,
												0.7,
												RefractiveIndex(1.8)},
									   1.f * m));
	scene.add(std::make_unique<Sphere>(Point3Df{5, 1, 0},
									   Material{ScaledColor(0.5, 0.6, 0.8),
												ScaledColor(0.3, 0.3, 0.5),
												ScaledColor(0, 0.3, 0.5),
												0.3,
												0.65,
												RefractiveIndex(1.6)},
									   1.f * m));
	/*for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			Point3Df center(a + 0.9 * random_float(),
							0.2,
							b + 0.9 * random_float());

			r.add(std::make_unique<Sphere>(
				center,
				Material{random_vector(),
						 random_vector(),
						 random_vector(),
						 random_float(),
						 random_float(),
						 RefractiveIndex(random_float(1, 6))},
				0.2f));
		}
	}*/

	scene.set_light_sources({LightSource{Point3Df{0, 5, 0},
										 ScaledColor{1, 0, 0},
										 ScaledColor{0, 0, 1}}});
	Image image = scene.rasterize(cam.orig(), 1920 * pixel, 1080 * pixel);
	auto err    = image.save("sphere.ppm", 2);
	if (err.has_value()) spdlog::info("Wrote successfully");
	else spdlog::error("PNG encoder error: {}", err.error());
}
} // namespace raytracing
