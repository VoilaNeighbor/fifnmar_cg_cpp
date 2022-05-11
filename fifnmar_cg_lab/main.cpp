#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include "board.hpp"
#include "utils.hpp"
#include "draw_line_controller.hpp"

GLFWwindow* g_window {};

int main() {
	bool kVsync = true;
	i32 kScrWidth = 800;
	i32 kScrHeight = 600;
	u32 kCursorRadius = 3;
	u32 kCursorLen = kCursorRadius * 2;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // For Apple

	g_window = glfwCreateWindow(kScrWidth, kScrHeight, "CG Lab with OpenGL", null, null);
	ensure(g_window);
	glfwMakeContextCurrent(g_window);
	glfwSetFramebufferSizeCallback(g_window, [](auto _, i32 width, i32 height) {
		glViewport(0, 0, width, height);
	});
	glfwSetErrorCallback([](i32 code, auto description) {
		fmt::print(stderr, "<GlfwError code={}>{}</GlfwError>\n", code, description);
	});

	set_cursor_laser_image(kCursorRadius, kCursorLen);

	DrawLineController draw_line_controller;
	g_cursor_click_signal.connect(draw_line_controller);
	glfwSetMouseButtonCallback(g_window, mouse_button_callback);

	ensure(gladLoadGL(glfwGetProcAddress));
	board::init(160, 120);

	glfwSwapInterval(kVsync);

	while (!glfwWindowShouldClose(g_window)) {
		glfwWaitEvents();
		board::render();
		glfwSwapBuffers(g_window);
	}
}
