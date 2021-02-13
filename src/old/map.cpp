#include <map.h>
#include <engine/tar.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <engine/render.h>
#include <engine/input.h>
#ifdef PLATFORM_PSX
#include <engine/file_loader_psx.h>
#endif

unsigned char* mapData;
unsigned int mapWidth;
unsigned int mapHeight;
MapHeader mapHeader;

bool loadMap(const char* filepath) {
	uint32_t fileSize;
	uint8_t* fileData;
#ifdef PLATFORM_PSX
	fileData = loadFileFromCD(filepath, &fileSize);
#else
	fileData = load_tar_entry(filepath, (int*)&fileSize);
#endif
	if (!fileData) {
		printf("Can't find map file %s \n", filepath);
		return false;
	}

	if (fileSize < sizeof(MapHeader)) {
		free(fileData);
		printf("Invalid map file %s \n", filepath);
		return false;
	}

	memcpy((void*)&mapHeader, fileData, sizeof(MapHeader));

	if (mapHeader.version != GAME_MAP_VERSION) {
		free(fileData);
		printf("Invalid map version %s \n", filepath);
		return false;
	}

	mapWidth = mapHeader.width;
	mapHeight = mapHeader.height;

	uint32_t mapDataSize = fileSize - sizeof(MapHeader);

	if (mapDataSize < (mapWidth * mapHeight)) {
		free(fileData);
		printf("Map Data too small %s \n", filepath);
		return false;
	}

	if (mapData) free(mapData);
	mapData = (uint8_t*)malloc(mapDataSize);
	memcpy((void*)mapData, fileData + sizeof(MapHeader), mapDataSize);
	free(fileData);

	return true;
}

void saveMap(const char* filepath) {
	uint32_t mapBufferSize = sizeof(MapHeader) + (mapWidth * mapHeight);
	uint8_t* mapBuffer = (uint8_t*)malloc(mapBufferSize);
	memcpy((void*)mapBuffer, &mapHeader, sizeof(MapHeader));
	memcpy((void*)&mapBuffer[sizeof(MapHeader)], mapData, mapWidth * mapHeight);
#ifdef PLATFORM_DESKTOP
	FILE *file;
	file = fopen(filepath, "w");
	fwrite(mapBuffer, mapBufferSize, 1, file);
	fclose(file);
#endif // PLATFORM_DESKTOP
}

void createMap(uint32_t width, uint32_t height, const char* name) {
	mapHeader.version = GAME_MAP_VERSION;
	mapHeader.width = width;
	mapHeader.height = height;
	mapWidth = mapHeader.width;
	mapHeight = mapHeader.height;
	if(strlen(name) < 52) memcpy(mapHeader.name, name, strlen(name));
	else memcpy(mapHeader.name, name, 52);
	if (mapData) free(mapData);
	mapData = (uint8_t*)malloc(mapWidth * mapHeight);
	memset(mapData, 0, mapWidth * mapHeight);
}

void renderMap() {
	if (mapData) {
		for (uint32_t y = 0; y < mapHeight; y++) {
			for (uint32_t x = 0; x < mapWidth; x++) {
				uint8_t tile_id = mapData[MAP_COORD(x, y)];
				uint8_t tile_tx_row = tile_id / MAP_TEXTURE_TILES_PER_ROW;
				uint8_t tile_tx_col = tile_id % MAP_TEXTURE_TILES_PER_ROW;
				draw_sprite_tx_offset_imageID(IMAGE_TERRAIN, TILE_SIZE, TILE_SIZE, tile_id * TILE_SIZE, 0, x * TILE_SIZE, y * TILE_SIZE);
			}
		}
	}
}

uint32_t cursorXTile() {
	int32_t worldCursorPos = cursorX + cameraX;
	uint32_t worldCursorTilePos = worldCursorPos / TILE_SIZE;
	if (worldCursorTilePos < 0) return 0;
	else if (worldCursorTilePos > mapWidth) return mapWidth - 1;
	else return worldCursorTilePos;
}

uint32_t cursotYTile() {
	int32_t worldCursorPos = cursorY + cameraY;
	uint32_t worldCursorTilePos = worldCursorPos / TILE_SIZE;
	if (worldCursorTilePos < 0) return 0;
	else if (worldCursorTilePos > mapWidth) return mapWidth - 1;
	else return worldCursorTilePos;
}

uint32_t mapCoordUnderCursor() { return MAP_COORD(cursorXTile(), cursotYTile()); }

