#include <engine/astar.h>
#include <engine/map.h>
#include <stdlib.h>

uint32_t mapWidth;
uint32_t mapHeight;

#ifndef SHRT_MIN
#define SHRT_MIN -(2^15 + 1)
#endif

void searchTileCostSort(TileCoordAstar *t) {
	for (uint32_t i = 0; i < 3; i++) {
		for (uint32_t i2 = 0; i2 < 3; i2++) {
			if (t[i2].cost > t[i2 + 1].cost) {
				TileCoordAstar temp = t[i2];
				t[i2] = t[i2 + 1];
				t[i2 + 1] = temp;
			}
		}
	}
}

//#define SEARCH_TILE_MAX_DEPTH 8

dynamicArray<TileCoord> closedTiles;

bool searchTile(Path& path, int16_t x, int16_t y, uint16_t endX, uint16_t endY, uint32_t depth, uint8_t maxDepth) {
	if (depth > maxDepth) return false;
	if ((x == endX) && (y == endY)) {
		path.steps.add({ x, y });
		return true;
	}
	closedTiles.add({ x, y });

	TileCoordAstar t[4];
	t[0].x = x + 1;
	t[0].y = y;

	if ((t[0].x < 0) || (t[0].x > static_cast<int16_t>(mapWidth)) || (t[0].y < 0) || (t[0].y > static_cast<int16_t>(mapHeight))) t[0].x = SHRT_MIN;

	t[1].x = x - 1;
	t[1].y = y;

	if ((t[1].x < 0) || (t[1].x > static_cast<int16_t>(mapWidth)) || (t[1].y < 0) || (t[1].y > static_cast<int16_t>(mapHeight))) t[1].x = SHRT_MIN;

	t[2].x = x;
	t[2].y = y + 1;

	if ((t[2].x < 0) || (t[2].x > static_cast<int16_t>(mapWidth)) || (t[2].y < 0) || (t[2].y > static_cast<int16_t>(mapHeight))) t[2].x = SHRT_MIN;

	t[3].x = x;
	t[3].y = y - 1;

	if ((t[3].x < 0) || (t[3].x > static_cast<int16_t>(mapWidth)) || (t[3].y < 0) || (t[3].y > static_cast<int16_t>(mapHeight))) t[3].x = SHRT_MIN;

	t[0].cost = 1 + abs(t[0].x - endX) + abs(t[0].y - endY);
	t[1].cost = 1 + abs(t[1].x - endX) + abs(t[1].y - endY);
	t[2].cost = 1 + abs(t[2].x - endX) + abs(t[2].y - endY);
	t[3].cost = 1 + abs(t[3].x - endX) + abs(t[3].y - endY);

	for (uint32_t i = 0; i < 4; i++) {
		if (t[i].x != SHRT_MIN) {
			if (map::activeMapTileData[MAP_COORD(t[i].x, t[i].y)] != 0) {
				t[i].cost *= 2;
				//t[i].x = SHRT_MIN;
			}
			for (uint32_t i2 = 0; i2 < closedTiles.getSize(); i2++) {
				TileCoord* tt = closedTiles.get(i2);
				if ((tt->x == t[i].x) && (tt->y == t[i].y)) {
					t[i].x = SHRT_MIN;
					break;
				}
			}
		}
	}

	searchTileCostSort(&t[0]);

	for (uint32_t i = 0; i < 4; i++) {
		if (t[i].x != SHRT_MIN) {
			if (searchTile(path, t[i].x, t[i].y, endX, endY, depth + 1, maxDepth)) {
				path.steps.add({ x, y });
				return true;
			}
		}
	}

	return false;
}

bool findPath(Path& path, uint16_t startX, uint16_t startY, uint16_t endX, uint16_t endY, uint8_t maxDepth) {
	mapWidth = map::activeMapHeader.width;
	mapHeight = map::activeMapHeader.height;

	path.steps.clear();
	closedTiles.clear();

	if (((startX == endX) && (startY == endY))) {
		path.steps.add({ (int16_t)startX, (int16_t)startY });
		return true;
	}
	else {
		return searchTile(path, startX, startY, endX, endY, 0, maxDepth);
	}
}
