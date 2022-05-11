#pragma once

#include "utils.hpp"
#include "signal.hpp"

struct CursorClickEvent {
	enum Button { Left, Right };

	// 0..=1
	f32 x, y;
	Button button;
};

extern ChainSignal<CursorClickEvent> g_cursor_click_signal;

struct GLFWwindow;
void mouse_button_callback(GLFWwindow* window, i32 button, i32 action, i32 mods);
