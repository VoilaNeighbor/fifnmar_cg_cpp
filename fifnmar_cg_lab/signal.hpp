#pragma once

#include "utils.hpp"

template <typename... Args>
struct ChainSignal;

template <typename... Args>
struct ChainSlotMixin {
	ChainSlotMixin(): prev(null), next(null) {}

	ChainSlotMixin(ChainSlotMixin* prev, ChainSlotMixin* next): prev(prev), next(next) {}

	virtual ~ChainSlotMixin() {
		if (prev) {
			prev->next = next;
		}
	}

	virtual void receive(Args...) = 0;

private:
	friend class ChainSignal<Args...>;

	ChainSlotMixin* prev;
	ChainSlotMixin* next;
};

template <typename... Args>
struct ChainSignal: private ChainSlotMixin<Args...> {
	void connect(ChainSlotMixin<Args...>& slot) {
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