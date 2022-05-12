#include "glfw_context.hpp"
#include "board.hpp"
#include "draw_line_controller.hpp"

int main() {
	glfw::init();
	board::init(160, 120);
	DrawLineController draw_line_controller;
	g_cursor_click_signal.connect(draw_line_controller);
	glfw::run(board::render);
}
