#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <fmt/core.h>
#include "board.hpp"
#include "utils.hpp"

int main() {
	int kScrWidth = 800;
	int kScrHeight = 600;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // For Apple

	auto window = glfwCreateWindow(kScrWidth, kScrHeight, "CG Lab with OpenGL", 0, 0);
	ensure(window);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, [](auto _, int width, int height) {
		glViewport(0, 0, width, height);
	});

	ensure(gladLoadGL(glfwGetProcAddress));
	board::init(80, 60);

	while (!glfwWindowShouldClose(window)) {
		glfwWaitEvents();
		board::render();
		glfwSwapBuffers(window);
	}
}
