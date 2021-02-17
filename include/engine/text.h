#pragma once

#include <CXXoverC/intMap.h>
#include <CXXoverC/string.h>

namespace text {
	//extern IntMap<string> texts;

	uint32_t utf8Convert(uint16_t* unicode_point, const char* text, uint32_t position);
}
