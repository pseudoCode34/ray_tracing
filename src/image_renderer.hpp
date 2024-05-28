#ifndef IMAGE_RENDERER_HPP
#define IMAGE_RENDERER_HPP

// IWYU pragma: no_include "vector3f.hpp"
#include "camera.hpp"
#include "constants/indexes_of_refraction.hpp"
#include "intersection.hpp"
#include "light_source.hpp"
#include "ray.hpp"
#include "solid_object_list.hpp"
#include "vector3f.hpp"
#include "viewport.hpp"

#include <lodepng.h>
#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/stopwatch.h>

namespace raytracing {
/**
 * \brief The Scene object renders a collection of SolidObjects and
 * LightSources that illuminate them.
 */
class ImageRenderer {
public:
	explicit ImageRenderer(Vector3fConstRef orig,
						   float ambient_ior    = ior::VACUUM,
						   uint8_t bounce_limit = 5);

	void add(std::unique_ptr<SolidObject> &&solid);

	void set_light_sources(LightSourceList &&light_source_list);

	// By default, regions of space that are not explicitly occupied by some
	// object have the refractive index of vacuum, or REFRACTION_VACUUM = 1.
	// The value of the 'refraction' parameter must be in the range
	// [REFRACTION_MINIMUM..REFRACTION_MAXIMUM].
	void set_ambient_refraction(float index);

	/**
	 * \brief Generate an image of the scene and write it to the .ppm format
	 *
	 * Color represented in floating point format must be clamped to [0,1] and
	 * transformed to RGB 255 format.
	 *
	 * \param dimension The resolution of the output image.
	 * \param filename A relative directory to write the image.
	 */
	void save_image(fmt::cstring_view filename, const Viewport &vp,
					Rect dimension);

	/**
	 * \brief Downsample the image buffer to an unsigned char array of RGBA
	 * values that LodePNG understands.
	 */
	void export_png(const std::string &filename, const Viewport &vp,
					Rect dimension) const;


	/**
	 * \brief Get a randomly-sampled camera ray for the pixel at location i,j,
	 * originating from the camera defocus disk.
	 *
	 * \param i pixel row-th
	 * \param j pixel height-th
	 */
	[[nodiscard]] Ray get_ray(const Viewport &viewport, size_t i,
							  size_t j) const;

	/**
	 * \brief Computes the local illumination color at a surface point based on
	 * ambient, diffuse, and specular reflection components using the Phong
	 * reflection model.
	 *
	 * Iterate through all light sources in the scene, looking for unblocked
	 * lines of sight to this intersection
	 *
	 * \param ka Ambient reflection coefficient.
	 * \param Ia Ambient light intensity.
	 * \param kd Diffuse reflection coefficient.
	 * \param L Light direction vector.
	 * \param N Surface normal vector.
	 * \param Il Diffuse light intensity.
	 * \param ks Specular reflection coefficient.
	 * \param R Reflected light direction vector.
	 * \param V Viewer direction vector.
	 * \param n Shininess coefficient.
	 *
	 * \return The computed local illumination color.
	 * FIXME: blinn-phong
	 */
	[[nodiscard]] ScaledColor
	local_illumination(const Intersection &intersection) const;

	/**
	 * \brief Returns true if the ray \a can hit no obstacle, False otherwise
	 *
	 * The closest intersection found is a blocker if it is closer to
	 * intersection point than the light source. Clearly, when the closest
	 * intersection is farther away than point2, conclude that there is no
	 * blocker
	 */
	[[nodiscard]] bool hit_any_obstacle(const Ray &r) const;

	/**
	 */
	[[nodiscard]] float target_refractive_index(Vector3fConstRef point) const;

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
	ScaledColor trace_ray(const Ray &ray, uint8_t bounce_count = 0) const;

private:
	/**
	 * \return if the number of light bounces has exceeded the maximum limit.
	 *
	 * \param bounce_count The depth of the recursion calls of light bouncing
	 * off so far before being assumed to diminish to nothing.
	 */
	constexpr bool exceeds_bounce_limit(uint8_t bounce_count) const {
		return bounce_count >= bounce_limit_;
	}

	LightSourceList light_source_list_;
	SolidObjectList world_;

	/*
	 * \brief The refractive index of every point in space that is not
	 * explicitly occupied by some object.
	 *
	 * Set to a higher value to simulate the entire scene being immersed in
	 * some transparent substance like water.
	 */
	float ambient_ior_;
	Vector3f orig_;
	// A limit to how deeply lights may go before it fades away.
	uint8_t bounce_limit_;
};

} // namespace raytracing
#endif /* ifndef IMAGE_RENDERER_HPP */
