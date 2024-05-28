#ifndef RECT_HPP
#define RECT_HPP

#include <cstddef>

namespace raytracing {
struct Rect {
	float width, height;

	[[nodiscard]] constexpr float aspect_ratio() const {
		return width / height;
	}

	[[nodiscard]] constexpr Rect scale_height(float height) const {
		return {height * aspect_ratio(), height};
	}

	[[nodiscard]] constexpr size_t area() const { return width * height; }
};
} // namespace raytracing
#endif /* ifndef RECT_HPP */
