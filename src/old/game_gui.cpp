#include <engine/gui.h>
#include <engine/render.h>
#include <engine/input.h>
#include <game_gui.h>
#include <config.h>
#include <stdlib.h>
#include <string.h>
#include <map.h>

GameGUI activeGUI = GameGUI::EDITOR_NEW_MAP;

#ifdef HAVE_EXIT_OPTION
#include <stdlib.h>

const char* main_menu_entries[] = {
	"Editor",
	"Settings",
	"Exit"
};

uint32_t main_menu_entries_number = 3;
#else
const char* main_menu_entries[] = {
	"Editor",
	"Settings",
};

uint32_t main_menu_entries_number = 2;
#endif // PLATFORM_DESKTOP
int32_t main_menu_selected = 0;

const char* editor_tile_selector_entries[] = {
	"0 - GRASS",
	"1 - WATER",
	"SAVE MAP"
};

uint32_t editor_tile_selector_entries_number = 3;
int32_t editor_tile_selector_selected = 0;
bool editor_active = false;

const char* editor_new_map_entries[] = {
	"Width: ",
	"Height: ",
	"Name: ",
	"Filepath: ",
	"Create"
};

uint32_t newMapWidth = 1;
uint32_t newMapHeight = 1;
char* newMapWidthC = nullptr;
char* newMapHeightC = nullptr;
char* newMapName = "Test Map";
char* newMapFilepath = "test_map.map";
char* newMapWidthDisplayC = nullptr;
char* newMapHeightDisplayC = nullptr;
char* newMapNameDisplayC = nullptr;
char* newMapFilepathDisplayC = nullptr;

uint32_t editor_new_map_entries_number = 5;
int32_t editor_new_map_selected = 0;

void gui_main() {
	if (activeGUI == GameGUI::MAIN_MENU) {
		draw_string_centered("Ducks of Apocalypse", 160, 20);
		if (text_menu(&main_menu_entries[0], main_menu_entries_number, main_menu_selected, 160, 120 - ((main_menu_entries_number * TEXT_MENU_LINE_HEIGHT) / 2))) {
			if (main_menu_selected == 0) activeGUI = GameGUI::EDITOR_TILE_SELECTOR;
#ifdef HAVE_EXIT_OPTION
			if (main_menu_selected == 3) exit(0);
#endif // PLATFORM_DESKTOP
		}
	}
	if (activeGUI == GameGUI::EDITOR_TILE_SELECTOR) {
		editor_active = true;
		if (text_menu(&editor_tile_selector_entries[0], editor_tile_selector_entries_number, editor_tile_selector_selected, 200, 10)) {
			if (editor_tile_selector_selected == (editor_tile_selector_entries_number - 1)) {
#ifdef SUPPORT_MAP_SAVING
				saveMap("myh_map.map");
#endif // SUPPORT_MAP_SAVING
			}
			else {
				activeGUI = GameGUI::NONE;
			}
		}
	}
	if (activeGUI == GameGUI::EDITOR_NEW_MAP) {
		if (newMapName == NULL) newMapName = (char*)malloc(30);
		if (newMapFilepath == NULL) newMapFilepath = (char*)malloc(30);
		if (newMapWidthC == NULL) newMapWidthC = (char*)malloc(30);
		if (newMapHeightC == NULL) newMapHeightC = (char*)malloc(30);
		if (newMapWidthDisplayC == NULL) newMapWidthDisplayC = (char*)malloc(30);
		if (newMapHeightDisplayC == NULL) newMapHeightDisplayC = (char*)malloc(30);
		if (newMapNameDisplayC == NULL) newMapNameDisplayC = (char*)malloc(30);
		if (newMapFilepathDisplayC == NULL) newMapFilepathDisplayC = (char*)malloc(30);

		itoa(newMapWidth, newMapWidthC, 10);
		itoa(newMapHeight, newMapHeightC, 10);

		strcpy(newMapWidthDisplayC, "Width: ");
		strcat(newMapWidthDisplayC, newMapWidthC);

		strcpy(newMapHeightDisplayC, "Height: ");
		strcat(newMapHeightDisplayC, newMapHeightC);

		strcpy(newMapNameDisplayC, "Name: ");
		strcat(newMapNameDisplayC, newMapName);

		strcpy(newMapFilepathDisplayC, "Filepath: ");
		strcat(newMapFilepathDisplayC, newMapFilepath);

		editor_new_map_entries[0] = newMapWidthDisplayC;
		editor_new_map_entries[1] = newMapHeightDisplayC;
		editor_new_map_entries[2] = newMapNameDisplayC;
		editor_new_map_entries[3] = newMapFilepathDisplayC;

		if (text_menu(&editor_new_map_entries[0], editor_new_map_entries_number, editor_new_map_selected, 200, 10)) {
			
		}
		if (editor_new_map_selected == 0) {
			if (keyState & KEY_LEFT) {
				if((newMapWidth - 1) > 0) newMapWidth--;
			}
			else if (keyState & KEY_RIGHT) {
				if ((newMapWidth + 1) < 256) newMapWidth++;
			}
		}
		if (editor_new_map_selected == 1) {
			if (keyState & KEY_LEFT) {
				if ((newMapHeight - 1) > 0) newMapHeight--;
			}
			else if (keyState & KEY_RIGHT) {
				if ((newMapHeight + 1) < 256) newMapHeight++;
			}
		}
	}

	if (keyState & KEY_ACTION) {
		if (editor_active) {
			if (editor_tile_selector_selected >= 0) {
				mapData[mapCoordUnderCursor()] = (uint8_t)editor_tile_selector_selected;
			}
		}
	}
	if (keyState & KEY_MENU) {
		activeGUI = GameGUI::EDITOR_TILE_SELECTOR;
	}

#ifdef REVERSED_DRAW_ORDER
	set_image(IMAGE_FONT);
#endif // REVERSED_DRAW_ORDER
}
