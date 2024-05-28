#ifndef RECT_HPP
#define RECT_HPP

#include <cstddef>

namespace raytracing {
struct Rect {
	size_t width, height;

	[[nodiscard]] constexpr float aspect_ratio() const {
		return static_cast<float>(width) / height;
	}

	[[nodiscard]] constexpr size_t area() const { return width * height; }
};
} // namespace raytracing
#endif /* ifndef RECT_HPP */
