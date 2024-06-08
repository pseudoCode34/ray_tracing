#include "image_renderer.hpp"

#include "color.hpp"
#include "constants/indexes_of_refraction.hpp"
#include "geometric.hpp"
#include "utility.hpp"
#include "viewport.hpp"

#include <cstdint>
#include <gsl/gsl-lite.hpp>
#include <lodepng.h>

namespace raytracing {
ImageRenderer::ImageRenderer(Vector3fConstRef orig,
							 ScaledColorConstRef background, float ambient_ior,
							 uint8_t bounce_limit)
	: ambient_ior_(ambient_ior),
	  orig_(orig),
	  background_(background),
	  bounce_limit_(bounce_limit) {
	gsl_Expects(is_in_range(ambient_ior, ior::MIN, ior::MAX));
	gsl_Expects(bounce_limit < 20);
}

void ImageRenderer::add(std::unique_ptr<SolidObject> &&solid) {
	world_.insert(std::move(solid));
}

void ImageRenderer::set_light_sources(LightSourceList &&light_source_list) {
	light_source_list_ = std::move(light_source_list);
}

void ImageRenderer::set_ambient_refraction(float index) {
	gsl_Expects(is_in_range(index, ior::MIN, ior::MAX));
	ambient_ior_ = index;
}

Ray ImageRenderer::get_ray(const Viewport &viewport, size_t i, size_t j) const {
	return standard_form_of(orig_, viewport.at(i, j), false);
}

void ImageRenderer::save_image(fmt::cstring_view filename, const Viewport &vp,
							   Rect dimension, float screen_gamma) {
	spdlog::stopwatch sw;
	const auto &[width, height] = dimension;
	auto out                    = fmt::output_file(filename);
	out.print("P3\n{} {}\n255\n", width, height);

	for (size_t j = 0; j < height; ++j) {
		for (size_t i = 0; i < width; ++i) {
			const Ray r = get_ray(vp, i, j);
			print(out, trace_ray(r), screen_gamma);
		}
	}
	spdlog::info("Writing to {} elapsed {} seconds", filename.c_str(), sw);
}

ScaledColor
ImageRenderer::local_illumination(Vector3fConstRef view,
								  const Intersection &intersection) const {
	using std::pow;

	const auto &[_, normal, material] = intersection;
	gsl_Expects(view.isUnitary());
	/* TODO: Write a test case in case of normal negated due to exiting
	 * refraction <03-06-24, Nguyễn Khắc Trường> */
	// gsl_Expects(view.dot(normal) > 0);

	ScaledColor total = material->ambient();
	for (const auto &light : light_source_list_) {
		// shift a little bit to exclude this object when casting shadow ray
		const auto shadow_ray
			= standard_form_of(intersection.shift(1e-3), light.position);
		if (hit_any_obstacle(shadow_ray)) continue;

		Vector3f halfway_dir = (view + shadow_ray.direction).normalized();
		float lambertian     = saturate(normal.dot(shadow_ray.direction));
		float spec
			= pow(saturate(normal.dot(halfway_dir)), material->shininess());
		ScaledColor diffuse
			= light.diffuse.cwiseProduct(material->diffuse()) * lambertian;
		ScaledColor specular
			= light.specular.cwiseProduct(material->specular()) * spec;

		total += diffuse + specular;
	}
	return total;
}

bool ImageRenderer::hit_any_obstacle(const Ray &shadow_ray) const {
	return world_.any_match([&shadow_ray](const auto &obstacle) {
		auto root = obstacle->hit(shadow_ray);
		if (std::isinf(root)) return false;
		/* TODO: Can this comparison be reduced to `t<1` even
		 * shadow_ray.direction.isUnitary() is false? <03-06-24, Nguyễn Khắc
		 * Trường> */
		return root < shadow_ray.direction.squaredNorm();
	});
}

float ImageRenderer::target_refractive_index(Vector3fConstRef point) const {
	const auto obstacle = world_.find_any_primary_container(point);
	if (!obstacle.has_value()) return ambient_ior_;
	return obstacle.value()->get_optics().get_refractive_index();
}

ScaledColor ImageRenderer::trace_ray(const Ray &ray,
									 uint8_t bounce_count) const {
	if (exceeds_bounce_limit(bounce_count)) return ScaledColor::Zero();

	auto intersection = world_.find_closest_intersection(ray);
	if (!intersection.has_value()) return background_;

	const Vector3f incidence = ray.direction.normalized();
	float eta = ambient_ior_ / intersection->material->get_refractive_index();

	// Ray hits the object's inner face
	if (intersection->normal.dot(incidence) > 0) {
		intersection->normal = -intersection->normal;
		eta                  = 1 / eta;
	}

	auto reflection_coeff = reflectance(incidence, intersection->normal, eta);

	// Negating for incidence pointing outward
	const ScaledColor local = local_illumination(-incidence, *intersection);
	// FIXME: When negate normal, this lower down intersection inside sphere
	const ScaledColor reflected
		= trace_ray(Ray{intersection->shift(1e-3),
						reflect(incidence, intersection->normal)},
					bounce_count + 1);
	const ScaledColor refracted
		= trace_ray(Ray{intersection->position,
						refract(incidence, intersection->normal, eta)},
					bounce_count + 1);
	Eigen::Matrix3f color;
	color << local, reflected, refracted;
	return color * intersection->material->light_contribution(reflection_coeff);
}

void ImageRenderer::export_png(const std::string &filename, const Viewport &vp,
							   Rect dimension, float screen_gamma) const {
	spdlog::stopwatch sw;
	// I prefer std::array here, but lodepng API requires std::vector
	std::vector<uint8_t> rgba_buffer(dimension.area());

	for (size_t j = 0; j < dimension.height; ++j)
		for (size_t i = 0; i < dimension.width; ++i) {
			const Ray r      = get_ray(vp, i, j);
			const auto final = to_rgb(trace_ray(r), screen_gamma);
			for (auto component : final) rgba_buffer.push_back(component);
		}
	const auto error = lodepng::encode(filename,
									   rgba_buffer,
									   dimension.width,
									   dimension.height,
									   LCT_RGB);
	if (error)
		spdlog::error("PNG encoder error: {}", lodepng_error_text(error));
	spdlog::info("Writing to {} elapsed {} seconds", filename, sw);
}

const SolidObjectList &ImageRenderer::object_list() const { return world_; }
} // namespace raytracing
