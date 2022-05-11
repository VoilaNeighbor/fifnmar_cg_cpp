#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <fmt/core.h>
#include <magic_enum.hpp>
#include <cmath>
#include "board.hpp"
#include "utils.hpp"
#include "signal.hpp"
#include "cursor.hpp"
#include "draw.hpp"

struct DrawLineController: ChainSlotMixin<CursorClickEvent> {
	void receive(CursorClickEvent click) override {
		fmt::print("<Click pos=({}, {}), button={}/>\n", click.x, click.y, magic_enum::enum_name(click.button));
		if (click.button == CursorClickEvent::Right) {
			_state = kIdle;
		} else if (_state == kIdle) {
			std::tie(_sx, _sy) = std::tie(click.x, click.y);
			_state = kStarted;
		} else {
			auto sx = (u32)(_sx * (f32)(board::width()));
			auto sy = (u32)(_sy * (f32)(board::height()));
			auto tx = (u32)(click.x * (f32)(board::width()));
			auto ty = (u32)(click.y * (f32)(board::height()));
			board::on_render.connect([sx, sy, tx, ty] {
				map_line(sx, sy, tx, ty, [](u32 x, u32 y) {
					board::set_pixel(x, y, kBlack);
				});
			});
			_state = kIdle;
		}
	}

private:
	f32 _sx {}, _sy {};
	enum State { kIdle, kStarted } _state = kIdle;
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
