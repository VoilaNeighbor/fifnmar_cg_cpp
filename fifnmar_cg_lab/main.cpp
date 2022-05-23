#include "glfw_context.hpp"
#include "board.hpp"
#include "draw_controller.hpp"
#include "ui.hpp"

int main() {
	glfw::init();
	ui::init();
	board::init(160, 120);
	DrawController draw_line_controller;
	g_cursor_click_signal.connect(draw_line_controller);
	glfw::run([] {
		board::render();

		// UI should be rendered at last.
		ui::render();
	});
}
