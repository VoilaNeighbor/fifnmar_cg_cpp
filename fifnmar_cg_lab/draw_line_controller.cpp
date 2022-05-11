#include "draw_line_controller.hpp"
#include "draw.hpp"

namespace {
	std::pair<u32, u32> percent_to_pixel(f32 x, f32 y) {
		auto rx = (u32)(x * (f32)(board::width()));
		auto ry = (u32)(y * (f32)(board::height()));
		return { rx, ry };
	}
} // namespace

DrawLineController::DrawLineController() {
	board::on_render.connect([this] {
		if (_state != Started) { return; }
		auto [sx, sy] = percent_to_pixel(_sx, _sy);
		// todo
	});
}

void DrawLineController::receive(CursorClickEvent click) {
	fmt::print("<Click pos=({}, {}), button={}/>\n", click.x, click.y, magic_enum::enum_name(click.button));
	if (click.button == CursorClickEvent::Right) {
		_state = Idle;
	} else if (_state == Idle) {
		std::tie(_sx, _sy) = std::tie(click.x, click.y);
		_state = Started;
	} else {
		auto [sx, sy] = percent_to_pixel(_sx, _sy);
		auto [tx, ty] = percent_to_pixel(click.x, click.y);
		board::on_render.connect([sx, sy, tx, ty] {
			map_line(sx, sy, tx, ty, [](u32 x, u32 y) {
				board::set_pixel(x, y, kBlack);
			});
		});
		_state = Idle;
	}
}
