#include "cursor.hpp"
#include "board.hpp"

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

void set_cursor_laser_image(u32 radius, u32 len) {
	std::vector cursor_pixels(len * len, kWhite);
	for (auto i = (i32)-radius; i != radius; ++i) {
		for (auto j = (i32)-radius; j != radius; ++j) {
			using namespace std;
			auto idx = (i + radius) * len + j + radius;
			auto alpha = (u8)(max((f64)0, cos(2 * hypot(i, j) / radius)) * 255);
			cursor_pixels[idx] = kRed.with_alpha(alpha);
		}
	}
	GLFWimage cursor_img {
		.width = (i32)len,
		.height = (i32)len,
		.pixels = (u8*)cursor_pixels.data()
	};
	auto cursor = glfwCreateCursor(&cursor_img, (i32)radius, (i32)radius);
	ensure(cursor);
	glfwSetCursor(g_window, cursor);
}