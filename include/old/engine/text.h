#pragma once

#include <config.h>
#ifdef PLATFORM_PSX
#include <psx/stdint.h>
#else
#include <stdint.h>
#include <stdbool.h>
#endif // PLATFORM_PSX
#include <stdio.h>

//Return number of bytes in the character
//Currently only support one and two byte UTF-8 characters
int utf8_convert(uint16_t* unicode_point, const char* text, int position);

//void load_font_data(const char* filename);
int get_char_width(uint16_t unicodeCode);

