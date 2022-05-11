#include "cursor.hpp"

#include <GLFW/glfw3.h>

ChainSignal<MouseButton> g_cursor_click_signal {};

void mouse_button_callback(GLFWwindow*, i32 button_code, i32 action, i32 mods) {
	if (action != GLFW_PRESS) { return; }

	MouseButton button;
	if (button_code == GLFW_MOUSE_BUTTON_LEFT) {
		button = MouseButton::Left;
	} else if (button_code == GLFW_MOUSE_BUTTON_RIGHT) {
		button = MouseButton::Right;
	} else {
		return;
	}

	g_cursor_click_signal.send(button);
}

std::pair<f64, f64> cursor_coordinate() {
	f64 pixel_x, pixel_y;
	glfwGetCursorPos(g_window, &pixel_x, &pixel_y);

	i32 scr_width, scr_height;
	glfwGetWindowSize(g_window, &scr_width, &scr_height);

	auto x = pixel_x / scr_width;
	auto y = 1 - pixel_y / scr_height;

	return { x, y };
}
