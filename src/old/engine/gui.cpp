#include <engine/input.h>
#include <engine/render.h>
#include <engine/img.h>
#include <engine/gui.h>
#include <stdio.h>

void draw_cursor() {
	draw_sprite_tx_offset_imageID(IMAGE_GUI, 8, 8, 0, 8, cursorX - 4, cursorY - 4);
}

#define MAX_TEXT_MENU_MAX_SELECT_SPEED 15

bool text_menu(const char** entries, uint32_t number_of_entries, int32_t &selected, uint32_t x, uint32_t y) {
	for (uint32_t i = 0; i < number_of_entries; i++) {
		uint32_t entry_string_width = string_width(entries[i]);
		uint32_t entry_y = y + (i * TEXT_MENU_LINE_HEIGHT);
		draw_string_centered(entries[i], x, entry_y);
		if (selected == i) {
			draw_string(">", x - (entry_string_width / 2) - 8, entry_y);
			draw_string("<", x + (entry_string_width / 2), entry_y);
		}
	}

	//printf("%i \n", Frames % MAX_TEXT_MENU_MAX_SELECT_SPEED);
#ifdef CONTINUES_INPUT
	if ((Frames % MAX_TEXT_MENU_MAX_SELECT_SPEED) == 0) {
#endif //CONTINUES_INPUT
		if (keyState & KEY_UP) {
			selected--;
		}
		else if (keyState & KEY_DOWN) {
			selected++;
		}
#ifdef CONTINUES_INPUT
	}
#endif //CONTINUES_INPUT

	if (selected == number_of_entries) selected = 0;
	if (selected < 0) selected = number_of_entries - 1;

	return keyState & KEY_ACTION;
}
