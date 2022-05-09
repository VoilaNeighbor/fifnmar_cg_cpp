#pragma once

#include <concepts>
#include "utils.hpp"

struct GLFWwindow;

template <typename... Args>
struct Signal;

template <typename... Args>
struct Slot {
	Slot(): prev(null), next(null) {}

	Slot(Slot* prev, Slot* next): prev(prev), next(next) {}

	virtual ~Slot() {
		if (prev) {
			prev->next = next;
		}
	}

	virtual void receive(Args... args) = 0;

private:
	friend class Signal<Args...>;

	Slot* prev;
	Slot* next;
};

template <typename... Args>
struct Signal: private Slot<Args...> {
	void connect(Slot<Args...>& slot) {
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
