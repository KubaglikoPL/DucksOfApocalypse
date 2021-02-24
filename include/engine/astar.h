#pragma once

#include <engine/stdint.h>
#include <CXXoverC/dynamicArray.h>

//A* Pathfinding algorithm

struct TileCoord {
	int16_t x;
	int16_t y;

	void copy(const TileCoord& c) {
		x = c.x;
		y = c.y;
	}
};

struct TileCoordAstar :public TileCoord {
	uint32_t cost;
};

struct Path {
	dynamicArray<TileCoord> steps;
};

bool findPath(Path &path, uint16_t startX, uint16_t startY, uint16_t endX, uint16_t endY, uint8_t maxDepth);
