#pragma once

#include <engine/stdint.h>

#define GAME_MAP_VERSION 1

//Map Format(Version 1)
//-Map Header
//-Map Tile Data

//32 byte header
#pragma pack(push, 1)
struct MapHeader {
	uint8_t version = GAME_MAP_VERSION;
	uint16_t width = 1;
	uint16_t height = 1;
	char name[26];
	uint8_t name_null_terminator = 0;
} __attribute__((packed));
#pragma pack(pop)

extern MapHeader activeMapHeader;
extern uint8_t* activeMapTileData;
