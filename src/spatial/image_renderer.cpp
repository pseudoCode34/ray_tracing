#include "image_renderer.hpp"

#include "geometric.hpp"
#include "spatial/intersection.hpp"
#include "spatial/light_source.hpp"
#include "spatial/primitive.hpp"
#include "spatial/solid_object_list.hpp"
#include "spatial/transformable/solid_object.hpp"
#include "utility.hpp"

#include <cmath>
#include <cstdint>
#include <gsl/gsl-lite.hpp>
#include <optional>

namespace raytracing {
class Viewport;

ImageRenderer::ImageRenderer(const Viewport &vp, ScaledColorConstRef background,
							 RefractiveIndex ambient_ior, uint8_t bounce_limit)
	: vp_(vp),
	  ambient_ior_(ambient_ior),
	  bounce_limit_(bounce_limit),
	  background_(background) {
	gsl_Expects(bounce_limit < 20);
}

void ImageRenderer::add(std::unique_ptr<SolidObject> &&solid) {
	world_.insert(std::move(solid));
}

void ImageRenderer::set_light_sources(LightSourceList &&light_source_list) {
	light_source_list_ = std::move(light_source_list);
}

void ImageRenderer::set_ambient_refraction(RefractiveIndex index) {
	ambient_ior_ = index;
}

ScaledColor
ImageRenderer::local_illumination(Vector3fConstRef view,
								  const Intersection &intersection) const {
	using std::pow;

	const auto &[position, normal, material] = intersection;
	gsl_Expects(view.isUnitary());
	/* TODO: Write a test case in case of normal negated due to exiting
	 * refraction <03-06-24, Nguyễn Khắc Trường> */
	gsl_Expects(view.dot(normal) > 0);

	ScaledColor total = material->ambient();
	for (const auto &light : light_source_list_) {
		// shift a little bit to exclude this object when casting shadow ray
		const auto shadow_ray = Ray3D::Through(position, light.position);
		if (hit_any_obstacle(shadow_ray)) continue;

		Vector3Df halfway_dir = (view + shadow_ray.direction()).normalized();
		float lambertian      = saturate(normal.dot(shadow_ray.direction()));
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

bool ImageRenderer::hit_any_obstacle(const Ray3D &shadow_ray) const {
	return world_.any_match([&shadow_ray](const auto &obstacle) {
		auto root = obstacle->hit(shadow_ray);
		if (!root.has_value()) return false;
		return root.value() < shadow_ray.direction().squaredNorm();
	});
}

RefractiveIndex
ImageRenderer::target_refractive_index(Vector3fConstRef point) const {
	const auto obstacle = world_.find_any_primary_container(point);
	if (!obstacle.has_value()) return ambient_ior_;
	return obstacle.value()->get_optics().get_refractive_index();
}

ScaledColor ImageRenderer::trace_ray(const Ray3D &ray,
									 uint8_t bounce_count) const {
	if (exceeds_bounce_limit(bounce_count)) return ScaledColor::Zero();

	auto intersection = world_.find_closest_intersection(ray);
	if (!intersection.has_value()) return background_;

	float eta = ambient_ior_ / intersection->material->get_refractive_index();

	// Ray hits the object's inner face
	if (intersection->normal.dot(ray.direction()) > 0) {
		intersection->normal = -intersection->normal;
		eta                  = 1 / eta;
	}

	auto reflection_coeff
		= reflectance(ray.direction(), intersection->normal, eta);

	// Negating direction for passing incidence pointing outward to Blinn-phong
	// model
	const ScaledColor local
		= local_illumination(-ray.direction(), *intersection);
	// FIXME: When negate normal, this lower down intersection inside sphere
	const ScaledColor reflected
		= trace_ray(Ray3D{intersection->shift(1e-3),
						  reflect(ray.direction(), intersection->normal)},
					bounce_count + 1);
	const ScaledColor refracted
		= trace_ray(Ray3D{intersection->position,
						  refract(ray.direction(), intersection->normal, eta)},
					bounce_count + 1);
	Eigen::Matrix3f color;
	color << local, reflected, refracted;
	return color * intersection->material->light_contribution(reflection_coeff);
}

const SolidObjectList &ImageRenderer::object_list() const { return world_; }

constexpr bool ImageRenderer::exceeds_bounce_limit(uint8_t bounce_count) const {
	return bounce_count >= bounce_limit_;
}
} // namespace raytracing
