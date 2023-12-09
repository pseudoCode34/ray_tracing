#include <fmt/core.h>
#include <gtest/gtest.h>
#include <lodepng.h>

namespace raytracing {
// Encode from raw pixels to disk with a single function call
// The image argument has width * height RGBA pixels or width * height * 4 bytes
void encode_one_step(const char *filename, std::vector<unsigned char> &image,
					 unsigned width, unsigned height) {
	// saves image to filename given as argument. Warning, this overwrites the
	// file without warning!
	if (auto error = lodepng::encode(filename, image, width, height))
		throw std::runtime_error(fmt::format("Encoder error {}: {}",
											 error,
											 lodepng_error_text(error)));
}

TEST(LodePNGTest, AlphaDisabled) {
	const size_t WIDTH = 512, HEIGHT = 512;
	std::vector<uint8_t> image(WIDTH * HEIGHT * 4);
	for (size_t y = 0; y < HEIGHT / 2; y++) {
		for (size_t x = 0; x < WIDTH / 2; x++) {
			image[4 * WIDTH * y + 4 * x + 0] = 128;
			image[4 * WIDTH * y + 4 * x + 1] = 255;
			image[4 * WIDTH * y + 4 * x + 2] = 128;
		}
		for (size_t x = WIDTH / 2; x < WIDTH; x++)
			image[4 * WIDTH * y + 4 * x + 0] = 128;
	}
	for (size_t y = HEIGHT / 2; y < HEIGHT; y++) {
		for (size_t x = 0; x < WIDTH / 2; x++) {
			image[4 * WIDTH * y + 4 * x + 0] = 255;
			image[4 * WIDTH * y + 4 * x + 1] = 255;
			image[4 * WIDTH * y + 4 * x + 2] = 128;
		}
		for (size_t x = WIDTH / 2; x < WIDTH; x++) {
			image[4 * WIDTH * y + 4 * x + 1] = 63;
			image[4 * WIDTH * y + 4 * x + 2] = 128;
		}
	}
	EXPECT_NO_THROW(
		encode_one_step("images/disable_alpha.png", image, WIDTH, HEIGHT););
}

TEST(LodePNGTest, MultipleAlpha) {
	const size_t WIDTH = 512, HEIGHT = 512;
	std::vector<uint8_t> image(WIDTH * HEIGHT * 4);
	for (size_t y = 0; y < HEIGHT / 2; y++) {
		for (size_t x = 0; x < WIDTH / 2; x++) {
			image[4 * WIDTH * y + 4 * x + 0] = 128;
			image[4 * WIDTH * y + 4 * x + 1] = 255;
			image[4 * WIDTH * y + 4 * x + 2] = 128;
			image[4 * WIDTH * y + 4 * x + 3] = 255;
		}
		for (size_t x = WIDTH / 2; x < WIDTH; x++) {
			image[4 * WIDTH * y + 4 * x + 0] = 128;
			image[4 * WIDTH * y + 4 * x + 1] = 0;
			image[4 * WIDTH * y + 4 * x + 2] = 0;
			image[4 * WIDTH * y + 4 * x + 3] = 172;
		}
	}
	for (size_t y = HEIGHT / 2; y < HEIGHT; y++) {
		for (size_t x = 0; x < WIDTH / 2; x++) {
			image[4 * WIDTH * y + 4 * x + 0] = 255;
			image[4 * WIDTH * y + 4 * x + 1] = 255;
			image[4 * WIDTH * y + 4 * x + 2] = 128;
			image[4 * WIDTH * y + 4 * x + 3] = 63;
		}
		for (size_t x = WIDTH / 2; x < WIDTH; x++) {
			image[4 * WIDTH * y + 4 * x + 1] = 63;
			image[4 * WIDTH * y + 4 * x + 2] = 128;
			image[4 * WIDTH * y + 4 * x + 3] = 128;
		}
	}
	EXPECT_NO_THROW(
		encode_one_step("images/mulitple_alpha.png", image, WIDTH, HEIGHT););
}

TEST(LodePNGTest, InvisibleRegions) {
	const size_t WIDTH = 512, HEIGHT = 512;
	std::vector<uint8_t> image(WIDTH * HEIGHT * 4);
	for (size_t y = 0; y < HEIGHT / 2; y++) {
		for (size_t x = 0; x < WIDTH / 2; x++) {
			image[4 * WIDTH * y + 4 * x + 0] = 128;
			image[4 * WIDTH * y + 4 * x + 1] = 255;
			image[4 * WIDTH * y + 4 * x + 2] = 128;
			image[4 * WIDTH * y + 4 * x + 3] = 255;
		}
		for (size_t x = WIDTH / 2; x < WIDTH; x++)
			image[4 * WIDTH * y + 4 * x + 0] = 128;
	}
	for (size_t y = HEIGHT / 2; y < HEIGHT; y++) {
		for (size_t x = 0; x < WIDTH / 2; x++) {
			image[4 * WIDTH * y + 4 * x + 0] = 255;
			image[4 * WIDTH * y + 4 * x + 1] = 255;
			image[4 * WIDTH * y + 4 * x + 2] = 128;
		}
		for (size_t x = WIDTH / 2; x < WIDTH; x++) {
			image[4 * WIDTH * y + 4 * x + 1] = 63;
			image[4 * WIDTH * y + 4 * x + 2] = 128;
			image[4 * WIDTH * y + 4 * x + 3] = 128;
			image[4 * WIDTH * y + 4 * x + 3] = 172;
		}
	}
	EXPECT_NO_THROW(
		encode_one_step("images/invisible_region.png", image, WIDTH, HEIGHT););
}

TEST(LodePNGTest, RGBAFormatError) {
	const size_t WIDTH = 512, HEIGHT = 512;
	std::vector<uint8_t> image(WIDTH * HEIGHT * 4);
	for (size_t y = 0; y < HEIGHT / 2; y++) {
		for (size_t x = 0; x < WIDTH / 2; x++) {
			image[4 * WIDTH * y + 4 * x + 0] = 128;
			image[4 * WIDTH * y + 4 * x + 1] = 255;
			image[4 * WIDTH * y + 4 * x + 2] = 128;
		}
		for (size_t x = WIDTH / 2; x < WIDTH; x++) {
			image[4 * WIDTH * y + 4 * x + 0] = 128;
			image[4 * WIDTH * y + 4 * x + 3] = 20;
		}
	}
	for (size_t y = HEIGHT / 2; y < HEIGHT; y++) {
		for (size_t x = 0; x < WIDTH / 2; x++) {
			image[4 * WIDTH * y + 4 * x + 0] = 255;
			image[4 * WIDTH * y + 4 * x + 1] = 300;
			image[4 * WIDTH * y + 4 * x + 2] = 128;
			image[4 * WIDTH * y + 4 * x + 3] = 63;
		}
		for (size_t x = WIDTH / 2; x < WIDTH; x++) {
			image[4 * WIDTH * y + 4 * x + 1] = 63;
			image[4 * WIDTH * y + 4 * x + 2] = 290;
			image[4 * WIDTH * y + 4 * x + 3] = 128;
		}
	}
	EXPECT_NO_THROW(
		encode_one_step("images/rgba_format_error.png", image, WIDTH, HEIGHT););
}
} // namespace raytracing
