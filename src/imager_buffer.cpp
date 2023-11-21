#include "imager_buffer.hpp"

#include <algorithm>
#include <array>
#include <numeric>
#include <range/v3/algorithm/max.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/view/transform.hpp>

namespace raytracing {
namespace Imager {
ImageBuffer::ImageBuffer(size_t pixel_width, size_t pixel_height)
	: pixel_width_(pixel_width),
	  pixel_height_(pixel_height),
	  array_(pixel_width * pixel_height) {}

PixelData &ImageBuffer::pixel(size_t i, size_t j) {
	if (i >= pixel_width_ || j >= pixel_height_)
		throw ImagerException("Pixel coordinate(s) out of bounds");
	return array_[j * pixel_width_ + i];
}

const PixelData &ImageBuffer::pixel(size_t i, size_t j) const {
	if (i >= pixel_width_ || j >= pixel_height_)
		throw ImagerException("Pixel coordinate access is out of bound");
	return array_[j * pixel_width_ + i];
}

size_t ImageBuffer::get_pixel_width() const { return pixel_width_; }

size_t ImageBuffer::get_pixel_height() const { return pixel_height_; }

int ImageBuffer::max_color_parameter() const {
	auto max = ranges::max(array_ | ranges::views::transform([](const auto &p) {
							   const auto &[r, g, b, a] = p.color;
							   return std::array<int, 3>({r, g, b});
						   })
						   | ranges::views::join);
	if (max == 0) max = 255;
	return max;
}

} // namespace Imager
} // namespace raytracing
