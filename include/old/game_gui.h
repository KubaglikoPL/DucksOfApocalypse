#pragma once

enum class GameGUI {
	NONE,
	MAIN_MENU,
	EDITOR_TILE_SELECTOR,
	EDITOR_NEW_MAP
};

extern GameGUI activeGUI;
extern bool editor_active;

void gui_main();
