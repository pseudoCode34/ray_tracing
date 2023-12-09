#ifndef IMAGE_BUFFER
#define IMAGE_BUFFER

#include "imager.hpp"

namespace raytracing::Imager {
/*
 * \brief Hold an image in memory as it is being rendered.
 *
 * Once calculated, the image in the buffer can be translated into a graphics
 * format like PNG.
 */
class ImageBuffer {
public:
	explicit ImageBuffer(size_t pixel_width, size_t pixel_height);

	/*
	 * \brief Returns a read/write reference to the pixel data for the specified
	 * column \param i and row \param j.
	 */
	PixelData &pixel(size_t i, size_t j);

	/*
	 * \brief Returns a read-only reference to the pixel data for the specified
	 * column \param i and row \param j.
	 */
	[[nodiscard]] const PixelData &pixel(size_t i, size_t j) const;

	[[nodiscard]] size_t get_pixel_width() const;

	[[nodiscard]] size_t get_pixel_height() const;

	// Find the maximum red, green, or blue value in the image for automatically
	// scaling the image brightness.
	[[nodiscard]] int max_color_parameter() const;

	PixelData &operator[](std::size_t x, std::size_t y);
	[[nodiscard]] const PixelData &operator[](std::size_t x,
											  std::size_t y) const;

private:
	size_t pixel_width_;  // the width of the image in pixels (columns).
	size_t pixel_height_; // the height of the image in pixels (rows).
	std::vector<PixelData> array_; // flattened array [pixelsWide * pixelsHigh].
};

} // namespace raytracing::Imager

#endif /* ifndef IMAGE_BUFFER */
