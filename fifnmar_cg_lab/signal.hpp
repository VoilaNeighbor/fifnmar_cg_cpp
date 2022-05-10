#pragma once

#include <concepts>
#include "utils.hpp"

struct GLFWwindow;

template <typename... Args>
struct Signal;

template <typename... Args>
struct SlotMixin {
	SlotMixin(): prev(null), next(null) {}

	SlotMixin(SlotMixin* prev, SlotMixin* next): prev(prev), next(next) {}

	virtual ~SlotMixin() {
		if (prev) {
			prev->next = next;
		}
	}

	virtual void receive(Args... args) = 0;

private:
	friend class Signal<Args...>;

	SlotMixin* prev;
	SlotMixin* next;
};

template <typename... Args>
struct Signal: private SlotMixin<Args...> {
	void connect(SlotMixin<Args...>& slot) {
		slot.prev = this;
		slot.next = this->next;
		this->next = &slot;
	}

	void send(Args... args) {
		for (auto i = this->next; i; i = i->next) {
			i->receive(args...);
		}
	}

private:
	void receive(Args...) override {}
};


struct CursorClick {
	enum Button { kLeft, kRight };

	// 0..=1
	f32 x, y;
	Button button;
};

extern Signal<CursorClick> g_cursor_click_signal;

void mouse_button_callback(GLFWwindow* window, i32 button, i32 action, i32 mods);
