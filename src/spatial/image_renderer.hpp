#ifndef IMAGE_RENDERER_HPP
#define IMAGE_RENDERER_HPP

#include "graphics/image.hpp"
#include "intersection.hpp"
#include "light_source.hpp"
#include "primitive.hpp"
#include "quantity/pixel.hpp"
#include "solid_object_list.hpp"
#include "spatial/refractive_index.hpp"
#include "viewport.hpp"

#include <cstdint>
#include <memory>
#include <mp-units/bits/quantity_concepts.h>
#include <mp-units/systems/isq/space_and_time.h>
#include <spdlog/spdlog.h>
#include <spdlog/stopwatch.h>
#include <utility>

namespace raytracing {
class SolidObject;
struct Intersection;

/**
 * \brief The Scene object renders a collection of SolidObjects and
 * LightSources that illuminate them.
 */
class ImageRenderer {
public:
	explicit ImageRenderer(const Viewport &vp, ScaledColorConstRef background,
						   RefractiveIndex ambient_ior = RefractiveIndex::AIR,
						   uint8_t bounce_limit        = 5);

	void add(std::unique_ptr<SolidObject> &&solid);

	void set_light_sources(LightSourceList &&light_source_list);

	void set_ambient_refraction(RefractiveIndex index);

	/**
	 * \brief Iteratet through each pixel, create a randomly-sampled camera ray
	 * for the pixel at location i,j originating from the camera defocus disk
	 * and calculate the pixel color based on that ray
	 */

	template <mp_units::QuantityOf<mp_units::isq::width> Width,
			  mp_units::QuantityOf<mp_units::isq::height> Height>
	Image rasterize(Vector3fConstRef orig, Width width, Height height) const {
		spdlog::stopwatch sw;

		Eigen::Tensor<float, 3, Eigen::RowMajor> color_buffer(
			height.force_numerical_value_in(pixel),
			width.force_numerical_value_in(pixel),
			3);
		for (Eigen::Index row = 0; row < color_buffer.dimension(0); ++row)
			for (Eigen::Index col = 0; col < color_buffer.dimension(1); ++col) {
				const Ray3D r    = Ray3D::Through(orig, vp_.at(col, row));
				const auto color = trace_ray(r);
				for (Eigen::Index channel = 0;
					 channel < color_buffer.dimension(2);
					 channel++)
					color_buffer(row, col, channel) = color.coeff(channel);
			}
		spdlog::info("Rasterizing elapsed {} seconds", sw);
		return Image{std::move(color_buffer)};
	}

	/**
	 * \brief Computes the color at a surface point based on ambient, diffuse,
	 * and specular reflection components using the Blinn-Phong reflection
	 * model.
	 *
	 * Iterate through all light sources in the scene, find one that makes the
	 * object not appear in the dark area.
	 *
	 * \param view Viewer direction vector.
	 * \param intersection Specify frag's position, normal, and component colour
	 * \return The computed local illumination color.
	 *
	 * \warning The prerequisite of all vectors passed to Blinn-phong model is
	 * view, normal, shadow ray, and halfway dir must all pointing outward.
	 */
	[[nodiscard]] ScaledColor
	local_illumination(Vector3fConstRef view,
					   const Intersection &intersection) const;

	/**
	 * \brief Returns true if the \a shadow_ray hit something before reaching
	 * the light, false otherwise
	 *
	 * The closest intersection found is a blocker if it is closer to
	 * intersection point than the light source. Clearly, when the closest
	 * intersection is farther away than point2, conclude that there is no
	 * blocker
	 */
	[[nodiscard]] bool hit_any_obstacle(const Ray3D &shadow_ray) const;

	/**
	 * TODO: Add docs for this function
	 */
	RefractiveIndex target_refractive_index(Vector3fConstRef point) const;

	/**
	 * \brief Determines the color from ray in scene in a pixel.
	 *
	 * Assume the light bouncing after a specific time will diminish to
	 * nothing(zero). If hit nothing, it means the ray continues on forever,
	 * returns the background color. If found the closest intersection, apply
	 * the equation for computing the color in a scene involving both reflection
	 * and refraction to assign the color of the pixel corresponding to that
	 * pixel.
	 */
	[[nodiscard]]
	ScaledColor trace_ray(const Ray3D &ray, uint8_t bounce_count = 0) const;

	[[nodiscard]] const SolidObjectList &object_list() const;

private:
	/**
	 * \return if the number of light bounces has exceeded the maximum limit.
	 *
	 * \param bounce_count The depth of the recursion calls of light bouncing
	 * off so far before being assumed to diminish to nothing.
	 */
	[[nodiscard]] constexpr bool
	exceeds_bounce_limit(uint8_t bounce_count) const;

	LightSourceList light_source_list_;
	SolidObjectList world_;
	Viewport vp_;
	/*
	 * \brief The refractive index of every point in space that is not
	 * explicitly occupied by some object.
	 *
	 * Set to a higher value to simulate the entire scene being immersed in
	 * some transparent substance like water.
	 */
	RefractiveIndex ambient_ior_;
	// A limit to how deeply lights may go before it fades away.
	uint8_t bounce_limit_;
	ScaledColor background_;
};

} // namespace raytracing
#endif /* ifndef IMAGE_RENDERER_HPP */
