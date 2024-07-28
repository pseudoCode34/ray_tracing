#include "image.hpp"

#include <fmt/os.h>
#include <gsl/gsl-lite.hpp>
#include <lodepng.h>
#include <fmt/core.h>
#include <limits>
#include <string>
#include <utility>

namespace raytracing {
Image::Image(size_type height, size_type width)
	: pixel_buffer_(height, width, RGB_CHANNEL) {}

Image::Image(const_reference x) : pixel_buffer_(x) {
	gsl_Expects(x.dimension(2) == RGB_CHANNEL);
}

Image::Image(element_type &&x) : pixel_buffer_(std::move(x)) {}

Image::size_type Image::height() const { return pixel_buffer_.dimension(0); }

Image::size_type Image::width() const { return pixel_buffer_.dimension(1); }

Image::size_type Image::channel() const { return pixel_buffer_.dimension(2); }

Eigen::Tensor<uint8_t, 3, Eigen::RowMajor>
Image::normalize(float screen_gamma) const {
	const auto gamma_corrected = pixel_buffer_.pow(1.f / screen_gamma);
	const auto clamped         = gamma_corrected.cwiseMax(0.f).cwiseMin(1.f);
	const auto to_rgb
		= (clamped * static_cast<float>(std::numeric_limits<uint8_t>::max()))
			  .cast<uint8_t>();

	return to_rgb;
}

std::expected<void, const char *> Image::save(const std::filesystem::path &path,
											  float screen_gamma) {
	gsl_Expects(screen_gamma >= 1);
	if (const auto extension = path.extension(); extension == ".ppm") {
		auto out = fmt::output_file(path.c_str());
		out.print("P3\n{} {}\n255\n", width(), height());
		auto flattend = normalize(screen_gamma);
		for (size_type i = 0; i < height(); i++)
			for (size_type j = 0; j < width(); j++) {
				out.print("{} {} {}\n",
						  flattend(i, j, 0),
						  flattend(i, j, 1),
						  flattend(i, j, 2));
			}
		return {};
	}

	auto error = lodepng::encode(path.c_str(),
								 normalize(screen_gamma).data(),
								 width(),
								 height());
	if (error) return std::unexpected(lodepng_error_text(error));
	return {};
}

} // namespace raytracing
