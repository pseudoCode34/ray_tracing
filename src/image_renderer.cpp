#include "image_renderer.hpp"

#include "geometric.hpp"
#include "viewport.hpp"

#include <gsl/gsl-lite.hpp>

namespace raytracing {
ImageRenderer::ImageRenderer(Vector3fConstRef orig, float ambient_ior,
							 uint8_t bounce_limit)
	: ambient_ior_(ambient_ior), orig_(orig), bounce_limit_(bounce_limit) {
	gsl_Expects(ior::MIN <= ambient_ior && ambient_ior <= ior::MAX);
	gsl_Expects(bounce_limit < 20);
}

void ImageRenderer::add(std::unique_ptr<SolidObject> &&solid) {
	world_.insert(std::move(solid));
}

void ImageRenderer::set_light_sources(LightSourceList &&light_source_list) {
	light_source_list_ = std::move(light_source_list);
}

void ImageRenderer::set_ambient_refraction(float index) {
	gsl_Expects(ior::MIN <= index && index <= ior::MAX);
	ambient_ior_ = index;
}

Ray ImageRenderer::get_ray(const Viewport &viewport, size_t i, size_t j) const {
	return standard_form_of(orig_, viewport.at(i, j), false);
}

void ImageRenderer::save_image(fmt::cstring_view filename, const Viewport &vp,
							   Rect dimension) {
	spdlog::stopwatch sw;
	auto out = fmt::output_file(filename);
	out.print("P3\n{} {}\n255\n", dimension.width, dimension.height);

	// i=872 412
	for (size_t j = 0; j < dimension.height; ++j) {
		for (size_t i = 0; i < dimension.width; ++i) {
			const Ray r         = get_ray(vp, i, j);
			const ScaledColor x = trace_ray(r).cwiseMin(1).cwiseMax(0) * 255;
			print(out, x.cast<uint8_t>());
		}
	}
	spdlog::info("Writing to {} elapsed {} seconds", filename.c_str(), sw);
}

ScaledColor
ImageRenderer::local_illumination(const Intersection &intersection) const {
	ScaledColor sum = intersection.material->attenuation();

	for (const auto &light : light_source_list_)
		if (Ray r = standard_form_of(intersection.position, light.position);
			hit_any_obstacle(r)) {
			// TODO: Where is diffuse coeff?
			sum += intersection.normal.dot(r.direction) * light.color;
		}
	return sum;
}

bool ImageRenderer::hit_any_obstacle(const Ray &r) const {
	return world_.none_match([&r](const auto &obstacle) {
		const auto closest = obstacle->hit(r);

		if (!std::isinf(closest)) return false;
		return closest > r.direction.squaredNorm();
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
	if (!intersection.has_value()) return ScaledColor::Ones();
	const Vector3f incidence = ray.direction.normalized();

	float eta = ambient_ior_ / intersection->material->get_refractive_index();

	// Ray hits the object's inner face
	if (intersection->normal.dot(incidence) > 0) {
		intersection->normal = -intersection->normal;
		eta                  = 1 / eta;
	}

	auto reflection_coeff = reflectance(incidence, intersection->normal, eta);

	const ScaledColor local = local_illumination(*intersection);
	// FIXME: When negate normal, this lower down intersection inside sphere
	const ScaledColor reflected = trace_ray(
		Ray{intersection->position, reflect(incidence, intersection->normal)},
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
							   Rect dimension) const {
	spdlog::stopwatch sw;
	// I prefer std::array here, but lodepng API requires std::vector
	std::vector<uint8_t> rgba_buffer(dimension.area());

	for (size_t j = 0; j < dimension.height; ++j)
		for (size_t i = 0; i < dimension.width; ++i) {
			Ray r         = get_ray(vp, i, j);
			ScaledColor x = trace_ray(r).cwiseMin(1).cwiseMax(0) * 255;
			for (auto component : x.cast<uint8_t>())
				rgba_buffer.push_back(component);
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

} // namespace raytracing
