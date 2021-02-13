#pragma once

#include <config.h>
#ifdef PLATFORM_PSX
#include <psx/stdint.h>
#else
#include <stdint.h>
#include <stdbool.h>
#endif // PLATFORM_PSX

extern unsigned char* mapData;
extern unsigned int mapWidth;
extern unsigned int mapHeight;

#define MAP_COORD(X, Y) X + (Y * mapWidth)
#define GAME_MAP_VERSION 1
#define MAP_TEXTURE_TILES_PER_ROW 8
#define TILE_SIZE 16

struct MapHeader {
	uint32_t version;
	uint32_t width;
	uint32_t height;
	char name[52]; //Include NULL Terminator
};

bool loadMap(const char *filepath);
void saveMap(const char* filepath);
void createMap(uint32_t width, uint32_t height, const char* name);
void renderMap();
uint32_t cursorXTile();
uint32_t cursotYTile();
uint32_t mapCoordUnderCursor();