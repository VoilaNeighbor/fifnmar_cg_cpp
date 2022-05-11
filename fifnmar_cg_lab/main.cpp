#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include "board.hpp"
#include "utils.hpp"
#include "draw_line_controller.hpp"

int main() {
	i32 kScrWidth = 800;
	i32 kScrHeight = 600;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // For Apple

	auto window = glfwCreateWindow(kScrWidth, kScrHeight, "CG Lab with OpenGL", null, null);
	ensure(window);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, [](auto _, i32 width, i32 height) {
		glViewport(0, 0, width, height);
	});

	// Make a laser-pen cursor
	u32 constexpr kCursorRadius = 3;
	u32 constexpr kCursorLen = kCursorRadius * 2;
	Rgba cursor_pixels[kCursorLen * kCursorLen];
	for (i32 i = -kCursorRadius; i != kCursorRadius; ++i) {
		for (i32 j = -kCursorRadius; j != kCursorRadius; ++j) {
			using namespace std;
			auto idx = (i + kCursorRadius) * kCursorLen + j + kCursorRadius;
			auto alpha = (u8)(max((f64)0, cos(2 * hypot(i, j) / kCursorRadius)) * 255);
			cursor_pixels[idx] = kRed.with_alpha(alpha);
		}
	}
	GLFWimage cursor_img {
		.width = kCursorLen,
		.height = kCursorLen,
		.pixels = (u8*)cursor_pixels
	};
	auto cursor = glfwCreateCursor(&cursor_img, kCursorRadius, kCursorRadius);
	ensure(cursor);
	glfwSetCursor(window, cursor);

	DrawLineController draw_line_controller;
	g_cursor_click_signal.connect(draw_line_controller);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	ensure(gladLoadGL(glfwGetProcAddress));
	board::init(160, 120);

	while (!glfwWindowShouldClose(window)) {
		glfwWaitEvents();
		board::render();
		glfwSwapBuffers(window);
	}
}
