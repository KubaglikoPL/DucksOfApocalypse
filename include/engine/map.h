#pragma once

#include <engine/stdint.h>
#include <engine/graphics.h>

#define GAME_MAP_VERSION 1
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

	map::LoadResult loadMap(const char* filepath);
	void drawMap();
}