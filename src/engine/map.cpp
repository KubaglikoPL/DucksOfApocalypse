#include <engine/map.h>
#include <engine/file.h>
#include <engine/memory.h>
#include <string.h>

namespace map {
	map::MapHeader activeMapHeader;
	uint8_t* activeMapTileData = nullptr;
	image* terrainImage = nullptr;
}

map::LoadResult map::loadMap(const char* filepath) {
	if (activeMapTileData) free(activeMapTileData);

	uint32_t fileLen;
	uint8_t *fileData = loadFile(filepath, fileLen);
	if (!fileData) return map::LoadResult::FILE_NOT_FOUND;
	else {
		memcpy((void*)&map::activeMapHeader, fileData, sizeof(map::MapHeader));
		if (map::activeMapHeader.version != GAME_MAP_VERSION) {
			free(fileData);
			return map::LoadResult::MAP_INVALID_VERSION;
		}
		if ((map::activeMapHeader.width == 0) || (map::activeMapHeader.height == 0)) {
			free(fileData);
			return map::LoadResult::MAP_INVALID_DIMENSIONS;
		}
		if (map::activeMapHeader.name_null_terminator != 0) {
			free(fileData);
			return map::LoadResult::MAP_INVALID_NAME;
		}

		uint32_t mapTileDataSize = map::activeMapHeader.width * map::activeMapHeader.height;
		activeMapTileData = (uint8_t*)malloc(mapTileDataSize);
		memcpy((void*)activeMapTileData, (void*)&fileData[sizeof(map::MapHeader)], mapTileDataSize);
		free(fileData);
		return map::LoadResult::MAP_LOAD_SUCCESS;
	}
}

void map::drawMap() {
	if (activeMapTileData) {
		for (uint32_t y = 0; y < activeMapHeader.height; y++) {
			for (uint32_t x = 0; x < activeMapHeader.width; x++) {
				uint8_t tile = map::activeMapTileData[x + (y * activeMapHeader.width)];
				uint32_t texture_tile_row = tile / TERRAIN_TILE_TEXTURES_PER_ROW;
				uint32_t texture_tile_col = tile % TERRAIN_TILE_TEXTURES_PER_ROW;
				graphics::drawSprite(map::terrainImage, 16, 16, texture_tile_col * 16, texture_tile_row * 16, x * 16, y * 16);
			}
		}
	}
}
