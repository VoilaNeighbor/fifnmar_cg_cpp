#pragma once

#include "magic_enum.hpp"
#include "board.hpp"
#include "glfw_context.hpp"

struct DrawController: ChainSlotMixin<MouseButton> {
	enum Mode { Line, Circle, Oval };

	DrawController();
	void receive(MouseButton click) override;
	void set_mode(Mode mode);

	[[nodiscard]] Mode mode() const;

private:
	u32 _x0 {}, _y0 {};
	enum State { Idle, Started } _state = Idle;
	Mode _mode = Line;
};