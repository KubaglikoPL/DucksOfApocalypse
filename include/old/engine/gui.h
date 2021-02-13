#pragma once

#include <config.h>
#ifdef PLATFORM_PSX
#include <psx/stdint.h>
#else
#include <stdint.h>
#include <stdbool.h>
#endif // PLATFORM_PSX

#define TEXT_MENU_LINE_HEIGHT 14

void draw_cursor();
bool text_menu(const char** entries, uint32_t number_of_entries, int32_t &selected, uint32_t x, uint32_t y);
