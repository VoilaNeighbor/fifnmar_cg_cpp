#pragma once

#include "utils.hpp"

// Safety: the slop should be in 0..pi/4. The start point is origin.
void map_line_unsafe(u32 dx, u32 dy, std::invocable<u32, u32> auto f) {
	i32 u = 0;
	for (u32 x = 0, y = 0;; ++x) {
		f(x, y);
		if (x == dx) { break; }
		u += (i32)dy;
		if (u * 2 >= (i32)dx) {
			++y;
			u -= (i32)dx;
		}
	}
}

void map_line(u32 sx, u32 sy, u32 tx, u32 ty, std::invocable<u32, u32> auto f) {
	if (sx > tx) {
		std::swap(sx, tx);
		std::swap(sy, ty);
	}
	i32 v_flip = (sy > ty) ? -1 : 1;
	u32 dy = v_flip * (ty - sy);
	u32 dx = tx - sx;
	if (dy < dx) {
		map_line_unsafe(dx, dy, [=](u32 x, u32 y) { f(sx + x, sy + v_flip * y); });
	} else {
		map_line_unsafe(dy, dx, [=](u32 y, u32 x) { f(sx + x, sy + v_flip * y); });
	}
}