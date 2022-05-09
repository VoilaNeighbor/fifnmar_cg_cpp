#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <fmt/core.h>
#include "board.hpp"
#include "utils.hpp"


// Safety: the slop should be in 0..pi/4.
void map_line_unsafe(u32 sx, u32 sy, u32 dx, u32 dy, std::invocable<u32, u32> auto f) {
	i32 u = 0;
	for (u32 x = sx, y = sy;; ++x) {
		f(x, y);
		if (x == sx + dx) { break; }
		u += (i32)dy;
		if (u * 2 >= (i32)dx) {
			++y;
			u -= (i32)dx;
		}
	}
}

void map_line(u32 sx, u32 sy, u32 tx, u32 ty, std::invocable<u32, u32> auto f) {
	if (sx > tx) {
		std::swap(sx, tx);
		std::swap(sy, ty);
	}
	i32 v_flip = (sy > ty) ? -1 : 1;
	u32 dx = tx - sx;
	u32 dy = v_flip * (ty - sy);
	if (dy < dx) {
		map_line_unsafe(sx, sy, dx, dy, [f, v_flip](u32 x, u32 y) { f(x, v_flip * y); });
	} else {
		map_line_unsafe(sy, sx, dy, dx, [f, v_flip](u32 x, u32 y) { f(y, v_flip * x); });
	}
}

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
	glfwSetFramebufferSizeCallback(window, [](auto _, int width, int height) {
		glViewport(0, 0, width, height);
	});

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
