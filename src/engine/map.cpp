#include <engine/map.h>
#include <engine/file.h>
#include <engine/memory.h>
#include <engine/unit.h>
#include <string.h>

namespace map {
	map::MapHeader activeMapHeader;
	map::MapUnitTable activeMapUnitTable;
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
		uint32_t readCursor = sizeof(map::MapHeader);
		memcpy((void*)activeMapTileData, (void*)&fileData[readCursor], mapTileDataSize);
		readCursor += mapTileDataSize;

		printf("%i\n", fileData[readCursor]);
		activeMapUnitTable.numberOfUnits = ((uint16_t*)fileData)[readCursor / 2];
		uint32_t unitTableEntriesSize = activeMapUnitTable.numberOfUnits * sizeof(map::MapUnitEntry);
		if (activeMapUnitTable.entries) free(activeMapUnitTable.entries);
		activeMapUnitTable.entries = (map::MapUnitEntry*) malloc(unitTableEntriesSize);

		readCursor += 2;

		memcpy((void*)activeMapUnitTable.entries, (void*)&fileData[readCursor], unitTableEntriesSize);
		free(fileData);

		for (uint32_t i = 0; i < activeMapUnitTable.numberOfUnits; i++) {
			MapUnitEntry entry = activeMapUnitTable.entries[i];
			Unit unit;
			unit.hp = entry.hp;
			unit.type = entry.type;
			unit.x = entry.x * 16;
			unit.y = entry.y * 16;
			unit.data = unitsData.get(unit.type);
			unit.move = unit.data->move;
			units.add(unit);
		}

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
				graphics::drawSprite(map::terrainImage, 16, 16, texture_tile_col * 16, texture_tile_row * 16, x * 16, y * 16, false);
			}
		}
	}
}
