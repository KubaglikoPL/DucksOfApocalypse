#pragma once

#include <engine/stdint.h>
#include <engine/memory.h>
#include <string.h>
#include <CXXoverC/dynamicArray.h>

template<typename T>
struct IntPair {
	uint32_t key;
	T item;
};

template<typename T>
class IntMap {
	dynamicArray<IntPair<T>> items;
public:
	IntMap() {
		IntMap(0);
	}

	IntMap(uint32_t capacity) {
		if (capacity != 0) resize(capacity);
	}

	void add(uint32_t key, T item) {
		IntPair<T> pair;
		pair.key = key;
		pair.item = item;
		add(pair);
	}

	void add(IntPair<T> pair) {
		items.add(pair);
	}

	T* get(uint32_t key) {
		return items.get(getKeyIndex(key));
	}

	bool erase(uint32_t key) {
		return items.erase(getKeyIndex(key));
	}

	void clear() {
		items.clear();
	}

	uint32_t getSize() {
		return items.getSize();
	}

	void resize(uint32_t capacity) {
		items.resize(capacity);
	}

	uint32_t getKeyIndex(uint32_t key) {
		for (uint32_t i = 0; i < items.getSize(); i++) {
			if (items.get(i)->key == key) {
				return i;
			}
		}
		return UINT32_MAX;
	}
};
