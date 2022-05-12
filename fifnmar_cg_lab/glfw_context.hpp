#pragma once

#include <functional>
#include "color.hpp"
#include "signal.hpp"

enum class MouseButton { Left, Right };
extern ChainSignal<MouseButton> g_cursor_click_signal;

namespace glfw {
	void init();
	void run(std::function<void()> const& on_render);

	// Normalized within 0..1, could be out of range.
	std::pair<f64, f64> cursor_coordinate();
} // namespace glfw