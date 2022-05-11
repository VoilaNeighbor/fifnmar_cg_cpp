#pragma once

#include "utils.hpp"
#include "signal.hpp"

struct CursorClickEvent {
	enum Button { Left, Right };

	f32 x, y;
	Button button;
};

extern ChainSignal<CursorClickEvent> g_cursor_click_signal;

struct GLFWwindow;
void mouse_button_callback(GLFWwindow* window, i32 button_code, i32 action, i32 mods);

// Normalized within 0..1, could be out of range.
std::pair<f64, f64> cursor_coordinate();