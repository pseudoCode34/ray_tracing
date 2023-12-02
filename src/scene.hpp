#ifndef SCENCE_HPP
#define SCENCE_HPP

#include "color.hpp"
#include "imager_buffer.hpp"
#include "intersection.hpp"
#include "light_source.hpp"
#include "solid_object.hpp"

#include <expected>
#include <memory>
#include <optional>
#include <spdlog/sinks/basic_file_sink.h>
#include <stddef.h>
#include <stdint.h>
#include <vector>

namespace raytracing {
namespace Imager {
class ImageBuffer;
class SolidObject;
struct Vector;

enum class RayTracingError {
	MULTIPLE_MIN_DISTANCE,
	UNDEFINED_SOLID,
	REFRACTION_FAILURE
};

/*
 * \brief The Scene object renders a collection of SolidObjects and LightSources
 * that illuminate them.
 *
 * SolidObjects are added one by one using the method AddSolidObject. Likewise,
 * LightSources are added using AddLightSource.
 */
class Scene {
public:
	explicit Scene(const Color &background_color = {});
	Scene(const Scene &)            = default;
	Scene(Scene &&)                 = delete;
	Scene &operator=(const Scene &) = default;
	Scene &operator=(Scene &&)      = delete;

	// Caller must allocate solidObject via operator new.
	// This class will then own the responsibility of deleting it.
	/* [[nodiscard]] */
	SolidObject *add_solid_object(std::unique_ptr<SolidObject> &&solid_object);

	void add_light_source(LightSource &&light_source);

	/*
	 * \brief Generate an image of the scene and write it to the .png format
	 * with dimension of pixelsWide x * pixelsHigh.
	 *
	 * Renders an image of the current scene, with the camera at <0, 0, 0> and
	 * looking into the +z axis, with the +y axis upward.
	 *
	 * \param zoom_factor A postive number specifies the magnification level.
	 * Use 1.0 to start with, and try larger/smaller values to increase/decrease
	 * magnification. In other words, smaller values magnify the image more
	 * (zoom in), and larger values shrink all the scenery to fit more objects
	 * into the image (zoom out).
	 *
	 * \param png_file Directory of the PNG file to write the image to.
	 *
	 * \param pixelsWide, pixelsHigh Pixel dimensions of the output file.
	 *
	 * \param anti_alias_factor A multiplier for oversampling in an image.
	 * Adjust antiAliasFactor to increase the amount over oversampling to make
	 * smoother (less jagged) looking images. This causes run time and memory
	 * usage to increase O(N^2), so it is best to use a value between 1 (fastest
	 * but most "jaggy") to 4 (16 times slower but results in much smoother
	 * images).
	 */
	void save_image(const char *png_file, size_t pixels_width,
					size_t pixels_height, double zoom_factor,
					size_t anti_alias_factor) const;

	// By default, regions of space that are not explicitly occupied by some
	// object have the refractive index of vacuum, or REFRACTION_VACUUM = 1. The
	// following function allows the caller to override this default. The value
	// of the 'refraction' parameter must be in the range
	// [REFRACTION_MINIMUM..REFRACTION_MAXIMUM].
	void set_ambient_refraction(double refraction);

	void add_debug_point(size_t i_pixel, size_t j_pixel);

private:
	/*
	 * \brief Searches for an intersection with any solid in the scene from the
	 * vantage point in the given direction. If none are found, the function
	 * returns 0 and the 'intersection' parameter is left unchanged. Otherwise,
	 * returns the positive number of intersections that lie at minimal distance
	 * from the vantage point in that direction.
	 *
	 * Usually this number will be 1 (a unique intersection is closer than all
	 * the others) but it can be greater if multiple intersections are equally
	 * close (e.g. the ray hitting exactly at the corner of a cube could cause
	 * this function to return 3). If this function returns a value greater than
	 * zero, it means the 'intersection' parameter has been filled in with the
	 * closest intersection (or one of the equally closest intersections).
	 */
	IntersectionResult find_closest_intersection(const Vector &vantage,
												 const Vector &direction) const;

	// Returns true if nothing blocks a line drawn between point1 and point2.
	bool has_clear_line_of_sight(const Vector &point1,
								 const Vector &point2) const;

	/*
	 * \brief Determines whether the ray intersects with the surface of
	 * any of the objects in the scene. If none are found, we know that the ray
	 * continues on forever without hitting anything, and thus it can use the
	 * scene's background color for the ray. If one or more intersections along
	 * a given direction were found, pick the closest intersection and calls
	 * Scene::calculate_lighting to determine what color to assign to the pixel
	 * corresponding to that intersection
	 */
	std::expected<Color, RayTracingError> trace_ray(const Vector &vantage,
													const Vector &direction,
													double refractive_index,
													const Color &ray_intensity,
													int recursion_depth) const;

	/*
	 * \brief Determines the color of an intersection, based on illumination it
	 * receives via scattering, glossy reflection, and refraction (lensing).
	 *
	 * Given a point on the surface of some solid, and the direction from which
	 * it is seen, calculate_lighting uses physics equations to decide where
	 * that ray bounces, bends, and splits into multiple rays. It takes care of
	 * all this work so that the different classes derived from SolidObject can
	 * focus on surface equations and point containment. Because instances of
	 * SolidObject provide a common interface for finding intersections with
	 * surfaces of any shape, CalculateLighting can treat all solid objects as
	 * interchangeable parts, applying the same optical concepts in every case.
	 *
	 * \param intersection The intersection of a camera ray with the point on
	 * the surface that is closest to the camera in a particular \param
	 * direction from the vantage point
	 */
	std::expected<Color, RayTracingError>
	calculate_lighting(const Intersection &intersection,
					   const Vector &direction, double refractive_index,
					   const Color &ray_intensity, int recursion_depth) const;

	/*
	 * Determines the contribution of the illumination of a point based on matte
	 * (scatter) reflection based on light incident to a point on the surface of
	 * a solid object.
	 */
	Color calculate_matte(const Intersection &intersection) const;

	std::expected<Color, RayTracingError>
	calculate_reflection(const Intersection &intersection,
						 const Vector &incident_dir, double refractive_index,
						 const Color &ray_intensity, int recursion_depth) const;

	std::expected<Color, RayTracingError> calculate_refraction(
		const Intersection &intersection, const Vector &direction,
		double source_refractive_index, const Color &ray_intensity,
		int recursion_depth, double &out_reflection_factor) const;

	// FIXME: Use std::optional monadic operation
	SolidObject *primary_container(const Vector &point) const;

	double polarized_reflection(double source_material_refraction_index,
								double target_material_refraction_index,
								double cos_angle_of_incidence,
								double cos_outgoing_angle) const;

	void resolve_ambiguous_pixel(ImageBuffer &buffer, size_t i, size_t j) const;

	// Convert a floating point color component value, based on the maximum
	// component value, to a byte RGB value in the range 0x00 to 0xff.
	static uint8_t convert_pixel_value(double color_component,
									   double max_color_value);
	// The color to use for pixels where no solid
	// object intersection was found.
	Color background_color_;

	// Define types needed to hold a list of pixel coordinates.
	struct PixelCoordinates {
		size_t i, j;
	};

	// Define some list types used by member variables below.
	using SolidObjectList = std::vector<std::unique_ptr<SolidObject>>;
	using LightSourceList = std::vector<LightSource>;
	using PixelList       = std::vector<PixelCoordinates>;

	// A list of all the solid objects in the scene.
	SolidObjectList solid_object_list_;

	// A list of all the point light sources in the scene.
	LightSourceList light_source_list_;

	/*
	 * \brief The refractive index of every point in space that is not
	 * explicitly occupied by some object.
	 *
	 * By default, this is REFRACTION_VACUUM, but may be set to a higher value
	 * to simulate the entire scene being immersed in some transparent substance
	 * like water.
	 */
	double ambient_refraction_;

	struct DebugPoint {
		size_t i_pixel, j_pixel;
	};

	using DebugPointList = std::vector<DebugPoint>;
	DebugPointList debug_point_list_;
	mutable const DebugPoint *active_debug_point_;

	void export_file(const ImageBuffer &buffer, const char *png_file,
					 size_t pixel_width, size_t pixel_height,
					 double anti_alias_factor) const;

	ImageBuffer render(size_t pixel_width, size_t pixel_height,
					   double zoom_factor, size_t anti_alias_factor) const;

	std::vector<uint8_t> calculate_image(const ImageBuffer &buffer,
										 size_t pixel_width,
										 size_t pixel_height,
										 double anti_alias_factor) const;
};

extern std::shared_ptr<spdlog::logger> scene_logger;
} // namespace Imager
} // namespace raytracing

template <>
struct fmt::formatter<raytracing::Imager::RayTracingError>
	: formatter<string_view> {
	auto format(raytracing::Imager::RayTracingError error,
				format_context &ctx) const;
};

#endif /* ifndef SCENCE_HPP */
