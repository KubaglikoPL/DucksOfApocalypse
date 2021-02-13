#pragma once

#include <engine/stdint.h>
#include <engine/memory.h>
#include <string.h>
#include <stdio.h>

class string {
	char* buffer = nullptr;
	uint32_t length = 0;
	uint32_t capacity = 0;
public:
	const char* data() { return buffer; }
	uint32_t size() { return length; }
	void resize(uint32_t capacity) {
		if (buffer) {
			char* old_buffer = buffer;
			buffer = (char*)malloc(capacity + 1);
			buffer[capacity] = 0;
			memcpy(buffer, old_buffer, this->capacity);
			free(old_buffer);
		}
		else {
			buffer = (char*)malloc(capacity + 1);
			buffer[capacity] = 0;
		}

		this->capacity = capacity;
	}

	void clear() {
		length = 0;
		memset(buffer, 0, capacity);
	}

	void add(const char* str, uint32_t len) {
		if ((length + len) > capacity) {
			resize(length + len);
		}
		memcpy(&buffer[length], str, len);
		length += len;
	}

	void add(const char* str) {
		uint32_t str_len = strlen(str);
		add(str, str_len);
	}

	void add(string& str) {
		add(str.data());
	}

	void add(int32_t i) {
		char* buffer = (char*)malloc(10);
		memset(buffer, 0, 10);
		int str_len = snprintf(buffer, 10, "%i", i);
		add(buffer, str_len);
		free(buffer);
	}
};
