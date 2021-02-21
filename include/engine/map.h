#pragma once

#include <engine/stdint.h>
#include <engine/graphics.h>
#include <engine/gui.h>

#define GAME_MAP_VERSION 2
#define TERRAIN_TILE_TEXTURES_PER_ROW 16

//Map Format(Version 1)
//-Map Header
//-Map Tile Data

namespace map {
	enum class LoadResult {
		FILE_NOT_FOUND,
		MAP_INVALID_VERSION,
		MAP_INVALID_DIMENSIONS,
		MAP_INVALID_NAME,
		MAP_LOAD_SUCCESS
	};

	//32 byte header
#pragma pack(push, 1)
	struct MapHeader {
		uint8_t version = GAME_MAP_VERSION;
		uint16_t width = 1;
		uint16_t height = 1;
		char name[26];
		uint8_t name_null_terminator = 0;
	};
#pragma pack(pop)

	extern map::MapHeader activeMapHeader;
	extern uint8_t* activeMapTileData;
	extern image* terrainImage;

	struct MapUnitEntry {
		uint8_t type;
		uint8_t hp;
		uint16_t x;
		uint16_t y;
	};

	struct MapUnitTable {
		uint16_t numberOfUnits;
		MapUnitEntry* entries = nullptr;
	};

	extern MapUnitTable activeMapUnitTable;

	map::LoadResult loadMap(const char* filepath);
	void drawMap();

	enum class MapEditorGUI {
		CREATE_MAP_MENU,
		TILE_SELECT
	};

	/*class MapEditor {
		gui::TextMenu createMapMenu;
		gui::TextMenu tileSelectMenu;
		MapEditorGUI gui = MapEditorGUI::CREATE_MAP_MENU;
		string name;
		string filepath;
	public:
		MapEditor();
		void update();
	};*/
}
