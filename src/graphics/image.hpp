#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <stdint.h>
#include <expected>
#include <filesystem>
#include <unsupported/Eigen/CXX11/Tensor>
#include <Eigen/Core>

namespace raytracing {

static constexpr int TENSOR3D    = 3;
static constexpr int RGB_CHANNEL = 3;

class [[nodiscard]] Image {
public:
	using element_type    = Eigen::Tensor<float, TENSOR3D, Eigen::RowMajor>;
	using size_type       = element_type::Index;
	using value_type      = element_type::Scalar;
	using reference       = Eigen::TensorRef<element_type>;
	using const_reference = Eigen::TensorRef<const element_type>;

	explicit Image(size_type height, size_type width);

	explicit Image(const_reference x);

	explicit Image(element_type &&x);

	[[nodiscard]] size_type height() const;

	[[nodiscard]] size_type width() const;

	[[nodiscard]] size_type channel() const;

	/**
	 * \brief Transforming the scaled color in linear space to gamma space
	 * before converting to 255 format
	 *
	 * \param screen_gamma Represents the degree of correction applied. Common
	 * gamma values are around 2.2 for sRGB displays, which is a standard for
	 * many devices.
	 */
	[[nodiscard]] Eigen::Tensor<uint8_t, 3, Eigen::RowMajor>
	normalize(float screen_gamma = 2.2f) const;

	[[nodiscard]] std::expected<void, const char *>
	save(const std::filesystem::path &path, float screen_gamma = 2.2f);

private:
	element_type pixel_buffer_;
};

} // namespace raytracing
#endif /* ifndef IMAGE_HPP */
