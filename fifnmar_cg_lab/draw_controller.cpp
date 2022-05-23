#include "draw_controller.hpp"
#include "draw_generic.hpp"


DrawController::DrawController() {
	board::on_render([this] {
		if (_state != Started) { return; }
		auto [cx, cy] = glfw::cursor_coordinate();
		if (cx < 0 || cx >= 1 || cy < 0 || cy >= 1) { return; }

		auto x1 = (u32)(cx * board::width());
		auto y1 = (u32)(cy * board::height());

		map_line(_x0, _y0, x1, y1, [](u32 x, u32 y) {
			board::set_pixel(x, y, kSkyBlue);
		});
	});
}

void DrawController::receive(MouseButton click) {
	if (click == MouseButton::Right) {
		_state = Idle;
		return;
	}

	auto [cx, cy] = glfw::cursor_coordinate();
	auto x = (u32)(cx * board::width());
	auto y = (u32)(cy * board::height());
	if (_state == Idle) {
		_x0 = x;
		_y0 = y;
		_state = Started;
	} else {
		board::on_render([x0 = _x0, y0 = _y0, x1 = x, y1= y] {
			map_line(x0, y0, x1, y1, [](u32 x, u32 y) {
				board::set_pixel(x, y, kBlack);
			});
		});
		_state = Idle;
	}
}

void DrawController::set_mode(DrawController::Mode mode) { _mode = mode; }

DrawController::Mode DrawController::mode() const {
	return _mode;
}

