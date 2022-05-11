#pragma once

#include <magic_enum.hpp>
#include "board.hpp"
#include "cursor.hpp"

struct DrawLineController: ChainSlotMixin<CursorClickEvent> {
	DrawLineController();
	void receive(CursorClickEvent click) override;

private:
	f32 _sx {}, _sy {};
	enum State { Idle, Started } _state = Idle;
};
