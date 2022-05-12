#pragma once

#include "utils.hpp"

struct Rgba {
	u8 r, g, b, a;
	[[nodiscard]] Rgba with_alpha(u8 alpha) const {
		return { r, g, b, alpha };
	}
};

constexpr Rgba kRed = Rgba { 255, 0, 0, 255 };
constexpr Rgba kSkyBlue = Rgba { 135, 206, 235, 255 };
constexpr Rgba kWhite = Rgba { 255, 255, 255, 255 };
constexpr Rgba kBlack = Rgba { 0, 0, 0, 255 };
