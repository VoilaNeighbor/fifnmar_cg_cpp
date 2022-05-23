#include "glfw_context.hpp"

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "signal.hpp"
#include "color.hpp"

ChainSignal<MouseButton> g_cursor_click_signal {};

namespace glfw {
	GLFWwindow* raw_window;

	void init() {
		{ // Init GLFW
			ensure(glfwInit());
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // For Apple
			glfwSetErrorCallback([](i32 code, char const* desc) {
				fmt::print(stderr, "<GlfwError code={}>{}</GlfwError>\n", code, desc);
			});
		}
		{ // Make window
			raw_window = glfwCreateWindow(800, 600, "CG Lab with OpenGL", null, null);
			ensure(raw_window);
			glfwMakeContextCurrent(raw_window);
			glfwSetFramebufferSizeCallback(raw_window, [](auto, i32 w, i32 h) { glViewport(0, 0, w, h); });
			ensure(gladLoadGL(glfwGetProcAddress));
		}
		{ // Make cursor
			u32 kLen = 8;
			auto radius = (i32)kLen / 2;
			auto cursor_pixels = box<Rgba[]>(kLen * kLen);
			for (i32 i = -radius; i != radius; ++i) {
				for (i32 j = -radius; j != radius; ++j) {
					auto idx = (i + radius) * kLen + j + radius;
					auto alpha = (u8)(std::max((f64)0, cos(2 * hypot(i, j) / radius)) * 255);
					cursor_pixels[idx] = kRed.with_alpha(alpha);
				}
			}
			GLFWimage cursor_img {
				.width = (i32)kLen,
				.height = (i32)kLen,
				.pixels = (u8*)cursor_pixels.get()
			};
			auto cursor = glfwCreateCursor(&cursor_img, radius, radius);
			ensure(cursor);
			glfwSetCursor(raw_window, cursor);
		}
	}

	void run(std::function<void()> const& on_update) {
		while (!glfwWindowShouldClose(raw_window)) {
			glfwWaitEvents();
			on_update();
			glfwSwapBuffers(raw_window);
		}
	}

	std::pair<f64, f64> cursor_coordinate() {
		f64 pixel_x, pixel_y;
		glfwGetCursorPos(raw_window, &pixel_x, &pixel_y);

		i32 scr_width, scr_height;
		glfwGetWindowSize(raw_window, &scr_width, &scr_height);

		auto x = pixel_x / scr_width;
		auto y = 1 - pixel_y / scr_height;

		return { x, y };
	}
} // namespace glfw
