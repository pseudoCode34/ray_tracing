#include "scene.hpp"

#include "algebra.hpp"
#include "common_constants.hpp"
#include "imager.hpp"
#include "imager_buffer.hpp"
#include "lodepng.h"
#include "optics.hpp"
#include "solid_object.hpp"
#include "vector.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <memory>
#include <numeric>
#include <range/v3/algorithm/none_of.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/view/transform.hpp>
#include <span>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>
#include <utility>
#include <vector>

namespace raytracing::Imager {
auto scene_logger = spdlog::basic_logger_mt("scene", "logs/scene.txt");

Scene::Scene(const Color &background_color)
	: background_color_(background_color) {}

SolidObject *
Scene::add_solid_object(std::unique_ptr<SolidObject> &&solid_object) {
	solid_object_list_.push_back(std::move(solid_object));
	return solid_object_list_.back().get();
}

void Scene::add_light_source(LightSource &&light_source) {
	light_source_list_.push_back(std::move(light_source));
}

void Scene::set_ambient_refraction(double refraction) {
	refraction::validate_refraction(refraction);
	ambient_refraction_ = refraction;
}

void Scene::add_debug_point(size_t i_pixel, size_t j_pixel) {
	debug_point_list_.push_back(DebugPoint{i_pixel, j_pixel});
}

std::expected<Color, RayTracingError>
Scene::trace_ray(const Vector &vantage, const Vector &direction,
				 double refractive_index, const Color &ray_intensity,
				 int recursion_depth = 0) const {
	const auto &[intersection, err]
		= find_closest_intersection(vantage, direction);

	switch (err) {
	case IntersectionResultType::NOT_FOUND:
		// The ray of light did not hit anything. Therefore, we see the
		// background color attenuated by the incoming ray intensity.
		return ray_intensity * background_color_;

	case IntersectionResultType::UNIQUE:
	case IntersectionResultType::ACCEPTABLE:
		// The ray of light struck exactly one closest surface.
		// Determine the lighting using that single intersection.
		return calculate_lighting(intersection,
								  direction,
								  refractive_index,
								  ray_intensity,
								  recursion_depth + 1);
	case IntersectionResultType::AMBIGIOUS:
		return std::unexpected(RayTracingError::MULTIPLE_MIN_DISTANCE);
	}
}

bool is_recursion_limit_exceeded(int recursion_depth) {
	return recursion_depth > optical::RECURSION_LIMIT;
}

std::expected<Color, RayTracingError>
Scene::calculate_lighting(const Intersection &intersection,
						  const Vector &direction, double refractive_index,
						  const Color &ray_intensity,
						  int recursion_depth) const {
	Color color_sum;

#if RAYTRACE_DEBUG_POINTS
	if (active_debug_point_) {
		indent(recursion_depth);
		fmt::println("CalculateLighting[{}] (", recursion_depth);

		indent(1 + recursion_depth);
		fmt::println("{}", intersection);

		indent(1 + recursion_depth);
		fmt::println("direction = ", direction);

		indent(1 + recursion_depth);
		fmt::println("refract = {}, intensity = {}",
					 refractive_index,
					 ray_intensity);

		indent(recursion_depth);
		fmt::println(")");
	}
#endif

	// Check for recursion stopping conditions. The first is an absolute upper
	// limit on recursion, so as to avoid stack overflow crashes and to limit
	// computation time due to recursive branching.
	if (is_recursion_limit_exceeded(recursion_depth)) return {};

	// FIXME: Remove this limit
	// The second limit is checking for the ray path having been partially
	// reflected/refracted until it is too weak to matter significantly for
	// determining the associated pixel's color.
	/* if (!is_significant(ray_intensity)) return {}; */
	const auto &[distance_squared, point, surface_normal, solid, context, tag]
		= intersection;
	if (!solid)
		// Some derived class forgot to initialize intersection.solid before
		// appending to the intersection list.
		return std::unexpected(RayTracingError::UNDEFINED_SOLID);

	// Determine the optical properties at the specified point on whatever solid
	// object the ray intersected with.
	const Optics OPTICS = solid->surface_optics(point, context);

	// Opacity of a surface point is the fraction 0..1 of the light ray
	// available for matte and gloss. The remainder, transparency = 1-opacity,
	// is available for refraction and refractive reflection.
	const double OPACITY = OPTICS.get_opacity(), TRANSPARENCY = 1.0 - OPACITY;
	if (OPACITY > 0.0) {
		// This object is at least a little bit opaque,
		// so calculate the part of the color caused by
		// matte (scattered) reflection.
		const Color MATTE_COLOR = OPACITY * OPTICS.get_matte_color()
								  * ray_intensity
								  * calculate_matte(intersection);

		color_sum += MATTE_COLOR;

#if RAYTRACE_DEBUG_POINTS
		if (active_debug_point_) {
			indent(recursion_depth);
			fmt::println("matte_color = {}, color_sum = {}",
						 MATTE_COLOR,
						 color_sum);
		}
#endif
	}

	double refractive_reflection_factor = 0.0;
	if (TRANSPARENCY > 0.0) {
		// This object is at least a little bit transparent, so calculate
		// refraction of the ray passing through the point. The refraction
		// calculation also tells us how much reflection was caused by the
		// interface between the current ray medium and the medium it is now
		// passing into. This reflection factor will be combined with glossy
		// reflection to determine total reflection below.
		// Note that only the 'transparent' part of the light is available
		// for refraction and refractive reflection.

		// FIXME: Error handling the std::expected
		color_sum += calculate_refraction(
						 intersection,
						 direction,
						 refractive_index,
						 TRANSPARENCY * ray_intensity,
						 recursion_depth,
						 refractive_reflection_factor // output parameter
						 )
						 .value_or(Color{});
	}

	// There are 2 sources of shiny reflection in need of considering together:
	// 1. Reflection caused by refraction.
	// 2. The glossy part.

	// The refractive part causes reflection of all colors equally. Each color
	// component is diminished based on transparency (the part of the ray left
	// available to refraction in the first place).
	Color reflection_color("black");
	reflection_color *= TRANSPARENCY * refractive_reflection_factor;

	// Add in the glossy part of the reflection, which can be different for red,
	// green, and blue. It is diminished to the part of the ray that was not
	// available for refraction.
	reflection_color += OPACITY * OPTICS.get_gloss_color();

	// Multiply by the accumulated intensity of the ray as it has traveled
	// around the scene.
	reflection_color *= ray_intensity;

	// FIXME: this check has no effect when Color.red is an integer.
	/* if (is_significant(reflection_color)) { */
	auto matte_color = calculate_reflection(intersection,
											direction,
											refractive_index,
											reflection_color,
											recursion_depth);

	if (matte_color.has_value()) color_sum += matte_color.value();
	else spdlog::error("{}", matte_color.error());
		/* } */

#if RAYTRACE_DEBUG_POINTS
	if (active_debug_point_) {
		indent(recursion_depth);
		fmt::println("CalculateLighting[{}] returning {}",
					 recursion_depth,
					 color_sum);
	}
#endif

	return color_sum;
}

Color Scene::calculate_matte(const Intersection &intersection) const {
	auto combine_color = [&](Color sum, const LightSource &source) {
		// Since there is nothing between this point on the object's surface and
		// the given light source, add this light source's contribution based on
		// the light's color, luminosity, squared distance, and angle with the
		// surface normal.

		// Calculate a direction vector from the intersection point toward the
		// light source point.
		Vector direction = source.location - intersection.point;

		double incidence
			= intersection.surface_normal.dot(direction.unit_vector());

		// If the dot product of the surface normal vector and the ray toward
		// the light source is negative, it means light is hitting the surface
		// from the inside of the object, the very edge of the object.
		if (incidence <= 0.0) return sum;

		// Only when the dot product is positive does this light source
		// make the point brighter.
		double intensity = incidence / direction.magnitude_squared();
		return std::move(sum) + intensity * source.color;
	};
	const auto TOTAL_MATTE = ranges::accumulate(
		light_source_list_
			| ranges::views::filter([&](const LightSource &source) {
				  // See if we can draw a line from the intersection point
				  // toward the light source without hitting any surfaces.
				  return is_visible(intersection.point, source.location);
			  }),
		Color{},
		combine_color);
	/* scene_logger->info("Total matte is {}", TOTAL_MATTE); */
	return TOTAL_MATTE;
}

std::expected<Color, RayTracingError>
Scene::calculate_reflection(const Intersection &intersection,
							const Vector &incident_dir, double refractive_index,
							const Color &ray_intensity,
							int recursion_depth) const {
	// Find the direction of the reflected ray based on the incident ray
	// direction and the surface normal vector. The reflected ray has the same
	// angle with the normal vector as the incident ray, but on the opposite
	// side of the cone centered at the normal vector that sweeps out the
	// incident angle.
	const Vector &normal     = intersection.surface_normal;
	const double PERP        = incident_dir.dot(normal) * 2;
	const Vector REFLECT_DIR = incident_dir - (PERP * normal);

	// Follow the ray in the new direction from the intersection point.
	return trace_ray(intersection.point,
					 REFLECT_DIR,
					 refractive_index,
					 ray_intensity,
					 recursion_depth);
}

std::expected<Vector, RayTracingError>
find_refract_dir(std::span<const double> solutions,
				 const Vector &surface_normal, const Vector &dir_unit) {
	// There are generally 2 solutions for k, but only one of them is correct.
	// The right answer is the value of k that causes the light ray to bend the
	// smallest angle when comparing the direction of the refracted ray to the
	// incident ray. This is the same as finding the hypothetical refracted ray
	// with the largest positive dot product. In real refraction, the ray is
	// always bent by less than 90 degrees, so all valid dot products are
	// positive numbers.
	double max_alignment = -0.0001; // any negative number works as a flag
	Vector refract_dir;
	for (const auto &solution : solutions) {
		Vector refract_attempt = dir_unit + solution * surface_normal;
		double alignment       = dir_unit.dot(refract_attempt);
		if (alignment > max_alignment) {
			max_alignment = alignment;
			refract_dir   = refract_attempt;
		}
	}
	// There is something wrong with the math. Either there are no solutions to
	// the quadratic equation, or all solutions caused the refracted ray to bend
	// 90 degrees or more, which is not possible.
	if (max_alignment <= 0.0)
		return std::unexpected(RayTracingError::REFRACTION_FAILURE);
	return refract_dir;
}

std::expected<Color, RayTracingError>
Scene::calculate_refraction(const Intersection &intersection,
							const Vector &direction, double refraction_index,
							const Color &ray_intensity, int recursion_depth,
							double &out_reflection_factor) const {
	// Convert direction to a unit vector so that
	// relation between angle and dot product is simpler.
	const Vector DIR_UNIT = direction.unit_vector();

	double cos_a1 = DIR_UNIT.dot(intersection.surface_normal);
	assert("Dot product not too large." && cos_a1 <= +1. + EPSILON);
	assert("Dot product not too small." && cos_a1 >= -1. - EPSILON);
	double sin_a1 = sin_incident_ray_from(cos_a1);

	// The parameter sourceRefractiveIndex passed to this function tells us the
	// refractive index of the medium the light ray was passing through before
	// striking this intersection. We need to figure out what the target
	// refractive index is, i.e., the refractive index of whatever substance the
	// ray is about to pass into.  We determine this by pretending that the ray
	// continues traveling in the same direction a tiny amount beyond the
	// intersection point, then asking which solid object (if any) contains that
	// test point. Ties are broken by insertion order: whichever solid was
	// inserted into the scene first that contains a point is considered the
	// winner. If a solid is found, its refractive index is used as the target
	// refractive index; otherwise, we use the scene's ambient refraction, which
	// defaults to vacuum (but that can be overridden by a call to
	// Scene::SetAmbientRefraction).

	const double SMALL_SHIFT = 0.001;
	const Vector TEST_POINT  = intersection.point + SMALL_SHIFT * DIR_UNIT;
	const auto *container    = find_any_primary_container(TEST_POINT);
	const RefractionPair refractions{
		.src_coeff  = refraction_index,
		.dest_coeff = (container != nullptr) ? container->get_refractive_index()
											 : ambient_refraction_};

	const double RATIO = refraction_index / refraction_index;

	// Snell's Law: the sine of the refracted ray's angle with the normal is
	// obtained by multiplying the ratio of refractive indices by the sine
	// of the incident ray's angle with the normal.
	const double SIN_A2 = RATIO * sin_a1;

	if (SIN_A2 <= -1.0 || SIN_A2 >= +1.0) {
		// Since sin_a2 is outside the bounds -1..+1, then there is no such real
		// angle a2, which in turn means that the ray experiences total internal
		// reflection, so that no refracted ray exists.
		out_reflection_factor = 1.0; // complete reflection
		return Color{};              // no refraction at all
	}

	// Getting here means there is at least a little bit of refracted light in
	// addition to reflected light. Determine the direction of the refracted
	// light. We solve a quadratic equation to help us calculate the vector
	// direction of the refracted ray.

	const auto ROOTS
		= Algebra::solve_quadratic(1., cos_a1 * 2, 1. - 1. / (RATIO * RATIO));

	const auto refract_dir
		= find_refract_dir(ROOTS, intersection.surface_normal, DIR_UNIT);
	// FIXME: Propagate error through multiple callers
	if (!refract_dir.has_value()) return std::unexpected(refract_dir.error());

	const double cos_a2 = abs(sqrt(1. - SIN_A2 * SIN_A2));
	const CosinePair s_cosines{.incidence_angle = cos_a1,
							   .outgoing_angle  = cos_a2};
	const CosinePair p_cosines{.incidence_angle = cos_a2,
							   .outgoing_angle  = cos_a1};

	// Determine what fraction of the light is reflected at the
	// interface. The caller needs to know this for calculating total
	// reflection, so it is saved in an output parameter.

	// We assume uniform polarization of light, therefore average
	// the contributions of s-polarized and p-polarized light.
	const double RS = polarized_reflection(refractions, s_cosines);
	const double Rp = polarized_reflection(refractions, p_cosines);

	out_reflection_factor = std::midpoint(RS, Rp);

	// Whatever fraction of the light is NOT reflected goes into refraction.
	// The incoming ray intensity is thus diminished by this fraction.
	const Color NEXT_RAY_INTENSITY
		= (1.0 - out_reflection_factor) * ray_intensity;

	// Follow the ray in the new direction from the intersection point.
	return trace_ray(intersection.point,
					 refract_dir.value(),
					 refraction_index,
					 NEXT_RAY_INTENSITY,
					 recursion_depth);
}

double Scene::polarized_reflection(const RefractionPair &refractions,
								   const CosinePair &cosines) const {
	double left  = refractions.src_coeff * cosines.incidence_angle;
	double right = refractions.dest_coeff * cosines.outgoing_angle;
	double numer = left - right;
	double denom = left + right;

	const double COMPLETE_REFRACTION = 1.0;
	if (denom < EPSILON) // Assume complete reflection.
		return COMPLETE_REFRACTION;

	double reflection = (numer * numer) / (denom * denom);
	return std::max(COMPLETE_REFRACTION, reflection);
}

IntersectionResult
Scene::find_closest_intersection(const Vector &vantage,
								 const Vector &direction) const {
	const auto INTERSECTION_LIST
		= solid_object_list_ | ranges::views::transform([&](const auto &solid) {
			  return solid->append_all_intersections(vantage, direction);
		  })
		  | ranges::views::join | ranges::to<IntersectionList>();
	return pick_closest_intersection(INTERSECTION_LIST);
}

bool Scene::is_visible(const Vector &from, const Vector &to) const {
	auto block_between_2_points =
		[&from,
		 direction = to - from](const std::unique_ptr<SolidObject> &solid) {
			const auto &[closest, error]
				= solid->find_closest_intersection(from, direction);

			// We found the closest intersection, but it is only a blocker if it
			// is closer to \param from than \param to. If the closest
			// intersection is farther away than point2, there is nothing on
			// this object blocking the line of sight.
			return error != IntersectionResultType::NOT_FOUND
				   && closest.distance_squared < direction.magnitude_squared();
		};
	return ranges::none_of(solid_object_list_, block_between_2_points);
}

void Scene::save_image(const char *png_file, size_t pixel_width,
					   size_t pixel_height, double zoom_factor,
					   size_t anti_alias_factor) const {
	auto buffer
		= render(pixel_width, pixel_height, zoom_factor, anti_alias_factor);
	export_file(buffer, png_file, pixel_width, pixel_height, anti_alias_factor);
	spdlog::info("{}", png_file);
}

void Scene::handle_ambiguous_pixels(const PixelList &ambiguous_pixel_list,
									ImageBuffer &buffer) const {
	// Go back and "heal" ambiguous pixels as best we can.
	for (const auto [i, j] : ambiguous_pixel_list)
		resolve_ambiguous_pixel(buffer, i, j);
}

ImageBuffer Scene::render(size_t pixel_width, size_t pixel_height,
						  double zoom_factor, size_t anti_alias_factor) const {
	// Oversample the image using the anti-aliasing factor.
	const size_t LARGE_PIXEL_WIDTH  = anti_alias_factor * pixel_width;
	const size_t LARGE_PIXEL_HEIGHT = anti_alias_factor * pixel_height;
	const size_t SMALLER_DIM        = std::min(pixel_width, pixel_height);

	const double LARGE_ZOOM = anti_alias_factor * zoom_factor * SMALLER_DIM;
	ImageBuffer buffer(LARGE_PIXEL_WIDTH, LARGE_PIXEL_HEIGHT);
	// FIXME: What to do with background_color?

	// The camera is located at the origin.
	const Vector CAMERA{};

	// The camera faces in the -z direction. This allows the +x direction to
	// be to the right, and the +y direction to be upward.
	Vector direction{0.0, 0.0, -1.0};

	const Color FULL_INTENSITY{"black"};

	// We keep a list of (i,j) screen coordinates for pixels we are not able
	// to trace definitive rays for. Later we will come back and fix these
	// pixels.
	PixelList ambiguous_pixel_list;

	const double HALF_LARGE_WIDTH  = LARGE_PIXEL_WIDTH / 2.0;
	const double HALF_LARGE_HEIGHT = LARGE_PIXEL_HEIGHT / 2.0;
	for (size_t i = 0; i < LARGE_PIXEL_WIDTH; ++i) {
		// This cause underflow for size_t?
		direction.x = (i - HALF_LARGE_WIDTH) / LARGE_ZOOM;
		for (size_t j = 0; j < LARGE_PIXEL_HEIGHT; ++j) {
			direction.y = (HALF_LARGE_HEIGHT - j) / LARGE_ZOOM;

#if RAYTRACE_DEBUG_POINTS
			{
				// Assume no active debug point unless we find one below.
				active_debug_point_ = nullptr;

				for (const auto &iter : debug_point_list_)
					if (iter.i_pixel == i && iter.j_pixel == j) {
						fmt::println("Hit breakpoint at ({}, {})", i, j);
						active_debug_point_ = &iter;
						break;
					}
			}
#endif
			PixelData &pixel = buffer.pixel(i, j);
			// Trace a ray from the camera toward the given direction
			// to figure out what color to assign to this pixel.
			auto color = trace_ray(CAMERA,
								   direction,
								   ambient_refraction_,
								   FULL_INTENSITY);
			if (color.has_value()) pixel.color = color.value();
			else switch (color.error()) {
				case RayTracingError::MULTIPLE_MIN_DISTANCE:
				case RayTracingError::REFRACTION_FAILURE:
					scene_logger->error("{} at {}, {}", color.error(), i, j);

					// Something wrong in the recursive code for tracing rays,
					// there were multiple intersections that had minimum
					// distance from a vantage point. This can be really bad,
					// for example causing a ray of light to reflect inward into
					// a solid. Mark the pixel as ambiguous, so that any other
					// ambiguous pixels nearby know not to use it.
					pixel.is_ambiguous = true;

					// Keep a list of all ambiguous pixel coordinates to rapidly
					// enumerate through in the disambiguation pass.
					ambiguous_pixel_list.push_back(PixelCoordinates{i, j});
					break;
				case RayTracingError::UNDEFINED_SOLID:
					spdlog::error("{} at {}, {}", color.error(), i, j);
					break;
				default: std::unreachable();
				}
		}
	}
	/* for (size_t i = 0; i < LARGE_PIXEL_WIDTH; i++) */
	/* 	for (size_t j = 0; j < LARGE_PIXEL_HEIGHT; j++) { */
	/* 		const auto &[colour, _] = buffer.pixel(i, j); */
	/* 		scene_logger->info("{}", colour); */
	/* 	} */

#if RAYTRACE_DEBUG_POINTS
	// Leave no chance of a dangling pointer into debug points.
	active_debug_point_ = nullptr;
#endif
	handle_ambiguous_pixels(ambiguous_pixel_list, buffer);
	return buffer;
}

void Scene::export_file(const ImageBuffer &buffer, const char *png_file,
						size_t pixel_width, size_t pixel_height,
						size_t anti_alias_factor) const {
	const auto RGBA_BUFFER
		= calculate_image(buffer, pixel_width, pixel_height, anti_alias_factor);
	if (const auto ERROR
		= lodepng::encode(png_file, RGBA_BUFFER, pixel_width, pixel_height);
		ERROR) {
		spdlog::error("PNG encoder error: {}", lodepng_error_text(ERROR));
	}
}

Color average_color(const ImageBuffer &buffer, size_t anti_alias_factor,
					size_t i, size_t j) {
	const size_t PATCH_SIZE = anti_alias_factor * anti_alias_factor;
	Color color_sum;
	for (size_t di = 0; di < anti_alias_factor; ++di)
		for (size_t dj = 0; dj < anti_alias_factor; ++dj)
			color_sum += buffer[anti_alias_factor * i + di,
								anti_alias_factor * j + dj]
							 .color;
	return color_sum / PATCH_SIZE;
}

// Downsample the image buffer to an integer array of RGBA values that LodePNG
// understands.
std::vector<uint8_t> Scene::calculate_image(const ImageBuffer &buffer,
											size_t pixel_width,
											size_t pixel_height,
											size_t anti_alias_factor) const {
	// The number of bytes in buffer to be passed to LodePNG.
	constexpr size_t BYTES_PER_PIXEL = 4;
	const size_t RGBA_BUFFER_SIZE
		= pixel_width * pixel_height * BYTES_PER_PIXEL;

	// I prefer std::array here, but lodepng API requires std::vector
	std::vector<uint8_t> rgba_buffer(RGBA_BUFFER_SIZE);

	size_t rgba_index = 0;
	for (size_t j = 0; j < pixel_height; ++j)
		for (size_t i = 0; i < pixel_width; ++i) {
			Color average = average_color(buffer, anti_alias_factor, i, j);
			rgba_buffer[rgba_index++] = average.red;
			rgba_buffer[rgba_index++] = average.green;
			rgba_buffer[rgba_index++] = average.blue;
			rgba_buffer[rgba_index++] = MAX_COLOR_8BIT;
		}
	return rgba_buffer;
}

// The following function searches through all solid objects for the first
// solid (if any) that contains the given point. In the case of ties, the
// solid that was inserted into the scene first wins. This arbitrary
// convention allows the composer of a scene to decide which of multiple
// overlapping objects should control the index of refraction for any
// overlapping volumes of space.
const SolidObject *
Scene::find_any_primary_container(const Vector &point) const {
	for (const auto &solid_object : solid_object_list_)
		if (solid_object->contains(point)) return solid_object.get();
	return {};
}

// Average the colour of surrounding unambigiuous pixels.
Color average_unambiguous(const ImageBuffer &buffer, size_t i_min, size_t i_max,
						  size_t j_min, size_t j_max) {
	Color color_sum;
	size_t num_found = 0;
	for (auto si : ranges::views::iota(i_min, i_max))
		for (auto sj : ranges::views::iota(j_min, j_max)) {
			const PixelData &pixel = buffer[si, sj];
			if (!pixel.is_ambiguous) {
				++num_found;
				color_sum += pixel.color;
			}
		}

	if (num_found != 0) color_sum /= num_found;
	return color_sum;
}

void Scene::resolve_ambiguous_pixel(ImageBuffer &buffer, size_t i,
									size_t j) const {
	// This function is called whenever SaveImage could not figure out what
	// color to assign to a pixel, because multiple intersections were found
	// that minimize the distance to the vantage point.

	// Avoid going out of bounds with pixel coordinates.
	const size_t I_MIN = (i > 0) ? (i - 1) : i;
	const size_t I_MAX = (i < buffer.get_pixel_width() - 1) ? (i + 1) : i;
	const size_t J_MIN = (j > 0) ? (j - 1) : j;
	const size_t J_MAX = (j < buffer.get_pixel_height() - 1) ? (j + 1) : j;

	// "Airbrush" out the imperfection.
	// This is not perfect, but it looks better than leaving the pixel some
	// arbitrary color, and much better than picking the wrong intersection and
	// following it into a crazy direction.
	buffer[i, j].color
		= average_unambiguous(buffer, I_MIN, I_MAX, J_MIN, J_MAX);
}
} // namespace raytracing::Imager
