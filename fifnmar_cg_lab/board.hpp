#pragma once

#include <functional>
#include "signal.hpp"
#include "color.hpp"

namespace board {
	using RenderCallback = std::function<void()>;

	void init(u32 width, u32 height);
	void on_render(RenderCallback);
	void render();
	void set_pixel(u32 x, u32 y, Rgba color);
	u32 width();
	u32 height();
} // namespace board