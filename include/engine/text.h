#pragma once

#include <CXXoverC/intMap.h>
#include <CXXoverC/string.h>

#define LANG_ENGLISH 1
#define LANG_POLISH 2

#define TEXT_GUI_NEW_GAME 1
#define TEXT_GUI_LOAD_GAME 2
#define TEXT_GUI_OPTIONS 3
#define TEXT_GUI_EXIT 4
#define TEXT_GUI_CREDITS 5
#define TEXT_GUI_VERSION 6
#define TEXT_GUI_TITLE 7

namespace text {
	extern IntMap<string> texts_en;
	extern IntMap<string> texts_pl;
	extern uint8_t activeLang;
	
	string* getText(uint32_t textID);
	uint32_t utf8Convert(uint16_t* unicode_point, const char* text, uint32_t position);
}
