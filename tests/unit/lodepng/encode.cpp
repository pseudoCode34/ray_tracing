#include "lodepng.h"

#include <fmt/format.h>
#include <gtest/gtest.h>
#include <stdexcept>

// Encode from raw pixels to disk with a single function call
// The image argument has width * height RGBA pixels or width * height * 4 bytes
void encode_one_step(const char *filename, std::vector<unsigned char> &image,
					 unsigned width, unsigned height) {
	if (auto error = lodepng::encode(filename, image, width, height))
		throw std::runtime_error(fmt::format("Encoder error {}: {}",
											 error,
											 lodepng_error_text(error)));
}

// saves image to filename given as argument. Warning, this overwrites the file
// without warning!
TEST(LodePNGTest, SaveImage) {
	// generate some image
	size_t width = 512, height = 512;
	std::vector<uint8_t> image(width * height * 4);
	for (size_t y = 0; y < height; y++)
		for (size_t x = 0; x < width; x++) {
			image[4 * width * y + 4 * x + 0] = 255 * !(x & y);
			image[4 * width * y + 4 * x + 1] = x ^ y;
			image[4 * width * y + 4 * x + 2] = x | y;
			image[4 * width * y + 4 * x + 3] = 255;
		}
	EXPECT_NO_THROW(
		{ encode_one_step("images/lodepng.png", image, width, height); });
}
