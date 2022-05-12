#include "glfw_context.hpp"
#include "board.hpp"
#include "draw/draw_line_controller.hpp"
#include "ui.hpp"

int main() {
	glfw::init();
	ui::init();
	board::init(160, 120);
	DrawLineController draw_line_controller;
	g_cursor_click_signal.connect(draw_line_controller);
	glfw::run([] {
		board::render();
		ui::render();
	});
}
