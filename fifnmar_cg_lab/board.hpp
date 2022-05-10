#pragma once

#include "utils.hpp"

struct Rgba {
	u8 r, g, b, a;
	[[nodiscard]] Rgba with_alpha(u8 alpha) const {
		return { r, g, b, alpha };
	}
};

constexpr Rgba kRed = Rgba { 255, 0, 0, 255 };
constexpr Rgba kGreen = Rgba { 0, 255, 0, 255 };
constexpr Rgba kBlue = Rgba { 0, 0, 255, 255 };
constexpr Rgba kWhite = Rgba { 255, 255, 255, 255 };
constexpr Rgba kBlack = Rgba { 0, 0, 0, 255 };

namespace board {
	void init(u32 width, u32 height);
	void render();
	void set_pixel(u32 x, u32 y, Rgba color);
	u32 width();
	u32 height();
} // namespace board