#pragma once

#include <CXXoverC/intMap.h>
#include <CXXoverC/string.h>

#define LANG_ENGLISH 1
#define LANG_POLISH 2

namespace text {
	extern IntMap<string> texts_en;
	extern IntMap<string> texts_pl;
	extern uint8_t activeLang;
	
	string* getText(uint32_t textID);
	uint32_t utf8Convert(uint16_t* unicode_point, const char* text, uint32_t position);
}
