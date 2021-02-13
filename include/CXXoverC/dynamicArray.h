#pragma once

#include <engine/stdint.h>
#include <engine/memory.h>
#include <string.h>

template<typename T>
class dynamicArray {
	uint32_t capacity = 0;
	uint32_t size = 0;
	T* data = nullptr;
public:
	dynamicArray() {
		dynamicArray(0);
	}

	dynamicArray(uint32_t capacity) {
		if (capacity != 0) resize(capacity);
	}

	void resize(uint32_t capacity) {
		if (data) {
			T* old_data = data;
			data = (T*)malloc(capacity * sizeof(T));
			memcpy(data, old_data, this->capacity * sizeof(T));
			free(old_data);
		}
		else {
			data = (T*)malloc(capacity * sizeof(T));
		}

		this->capacity = capacity;
	}

	void add(T item) {
		if ((size + 1) > capacity) {
			if (capacity == 0) resize(1);
			else resize(capacity * 2);
		}

		data[size] = item;
		size++;
	}

	uint32_t getSize() {
		return size;
	}

	void clear() {
		size = 0;
	}

	T *get(uint32_t index) {
		if (index < size) {
			return &data[index];
		}
		else {
			return nullptr;
		}
	}

	bool erase(uint32_t index) {
		if ((index < size) && data) {
			T* new_data = (T*)malloc(capacity);
			uint32_t i2 = 0;
			for (uint32_t i = 0; i < size; i++) {
				if (i != index) {
					new_data[i2] = data[i];
					i2++;
				}
			}
			free(data);
			data = new_data;
			size--;
		}
		else {
			return false;
		}
	}
};
