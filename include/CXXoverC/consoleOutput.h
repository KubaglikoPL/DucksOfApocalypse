#pragma once

#include <CXXoverC/string.h>
#include <stdio.h>

class ConsoleOutput {
	string str;
public:
	ConsoleOutput* add(string& str) {
		this->str.add(str);
		return this;
	}

	ConsoleOutput* add(const char* str) {
		this->str.add(str);
		return this;
	}

	ConsoleOutput* add(const int32_t i) {
		this->str.add(i);
		return this;
	}

	ConsoleOutput* add(const uint32_t i) {
		this->str.add(i);
		return this;
	}

	ConsoleOutput* endl() {
		return add("\n");
	}

	void flush() {
		printf(str.data());
		str.clear();
	}
};

extern ConsoleOutput cout;