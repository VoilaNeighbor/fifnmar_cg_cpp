#pragma once

#include <magic_enum.hpp>
#include "board.hpp"
#include "cursor.hpp"

struct DrawLineController: ChainSlotMixin<MouseButton> {
	DrawLineController();
	void receive(MouseButton click) override;

private:
	u32 _x0 {}, _y0 {};
	enum State { Idle, Started } _state = Idle;
};
