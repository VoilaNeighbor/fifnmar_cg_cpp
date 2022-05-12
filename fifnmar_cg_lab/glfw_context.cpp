#include "glfw_context.hpp"

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "signal.hpp"
#include "color.hpp"

ChainSignal<MouseButton> g_cursor_click_signal {};

namespace glfw {
	static GLFWwindow* _window;

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
			_window = glfwCreateWindow(800, 600, "CG Lab with OpenGL", null, null);
			ensure(_window);
			glfwMakeContextCurrent(_window);
			glfwSetFramebufferSizeCallback(_window, [](auto, i32 w, i32 h) { glViewport(0, 0, w, h); });
			ensure(gladLoadGL(glfwGetProcAddress));
		}
		{ // Make cursor
			u32 kLen = 8;
			Rgba kColor = kRed;
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
			glfwSetCursor(_window, cursor);
		}
		glfwSetMouseButtonCallback(_window, [](auto, i32 button_code, i32 action, auto) {
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
		});
	}

	void run(std::function<void()> const& on_render) {
		while (!glfwWindowShouldClose(_window)) {
			glfwWaitEvents();
			on_render();
			glfwSwapBuffers(_window);
		}
	}

	std::pair<f64, f64> cursor_coordinate() {
		f64 pixel_x, pixel_y;
		glfwGetCursorPos(_window, &pixel_x, &pixel_y);

		i32 scr_width, scr_height;
		glfwGetWindowSize(_window, &scr_width, &scr_height);

		auto x = pixel_x / scr_width;
		auto y = 1 - pixel_y / scr_height;

		return { x, y };
	}
} // namespace glfw
