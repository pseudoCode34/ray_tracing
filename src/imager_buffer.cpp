#include "imager_buffer.hpp"

namespace raytracing {
namespace Imager {
ImageBuffer::ImageBuffer(size_t pixel_width, size_t pixel_height)
	: pixel_width_(pixel_width),
	  pixel_height_(pixel_height),
	  array_(pixel_width * pixel_height) {}

PixelData &ImageBuffer::pixel(size_t i, size_t j) {
	assert(i < pixel_width_ && j < pixel_height_);
	return array_[j * pixel_width_ + i];
}

const PixelData &ImageBuffer::pixel(size_t i, size_t j) const {
	assert(i < pixel_width_ && j < pixel_height_);
	return array_[j * pixel_width_ + i];
}

size_t ImageBuffer::get_pixel_width() const { return pixel_width_; }

size_t ImageBuffer::get_pixel_height() const { return pixel_height_; }

} // namespace Imager
} // namespace raytracing
