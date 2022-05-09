#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <fmt/core.h>
#include <magic_enum.hpp>
#include "board.hpp"
#include "utils.hpp"
#include "signal.hpp"
#include "draw.hpp"

struct DrawLineController: Slot<CursorClick> {
	void receive(CursorClick click) override {
		fmt::print("<Click pos=({}, {}), button={}/>\n", click.x, click.y, magic_enum::enum_name(click.button));
	}

private:
//	enum State { kIdle, kStarted };
//	u32 sx, sy;
};

int main() {
	int kScrWidth = 800;
	int kScrHeight = 600;

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

	DrawLineController fake;
	g_cursor_click_signal.connect(fake);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	ensure(gladLoadGL(glfwGetProcAddress));
	board::init(160, 120);
	u32 sx = 80;
	u32 sy = 60;
	u32 len = 20;
	map_line(sx, sy, sx + len, sy, [](u32 x, u32 y) { board::set_pixel(x, y, kBlack); });
	map_line(sx, sy, sx, sy + len, [](u32 x, u32 y) { board::set_pixel(x, y, kBlack); });
	map_line(sx, sy, sx + len / 2, sy + len, [](u32 x, u32 y) { board::set_pixel(x, y, kBlack); });
	map_line(sx, sy, sx + len, sy + len / 2, [](u32 x, u32 y) { board::set_pixel(x, y, kBlack); });

	while (!glfwWindowShouldClose(window)) {
		glfwWaitEvents();
		board::render();
		glfwSwapBuffers(window);
	}
}
