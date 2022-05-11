#include "cursor.hpp"

#include <GLFW/glfw3.h>

ChainSignal<CursorClickEvent> g_cursor_click_signal {};

void mouse_button_callback(GLFWwindow* window, i32 button, i32 action, i32 mods) {
	bool is_click_event = action == GLFW_PRESS
		&& (button == GLFW_MOUSE_BUTTON_LEFT || button == GLFW_MOUSE_BUTTON_RIGHT);
	if (!is_click_event) { return; }

	f64 pixel_x, pixel_y;
	glfwGetCursorPos(window, &pixel_x, &pixel_y);

	i32 scr_width, scr_height;
	glfwGetWindowSize(window, &scr_width, &scr_height);

	auto x = pixel_x / scr_width;
	auto y = 1 - pixel_y / scr_height;

	auto button_type = button == GLFW_MOUSE_BUTTON_LEFT ? CursorClickEvent::Left : CursorClickEvent::Right;
	g_cursor_click_signal.send({ .x = (f32)x, .y = (f32)y, .button = button_type });
}
