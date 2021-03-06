#pragma once

#include <CXXoverC/intMap.h>

#define UNIT_ATTACK_TYPE_MEELE 1

#define UNIT_TYPE_BASIC 1

struct UnitData {
	uint32_t textID;
	uint8_t maxHP;
	uint8_t move;
	uint8_t attackType;
	uint8_t minAttack;
	uint8_t maxAttack;
	uint8_t type;
	uint8_t armor;
	uint8_t range;
	uint8_t textureX;
	uint8_t textureY;

	void copy(const UnitData& d) {
		textID = d.textID;
		maxHP = d.maxHP;
		move = d.move;
		attackType = d.attackType;
		minAttack = d.minAttack;
		maxAttack = d.maxAttack;
		type = d.type;
		armor = d.armor;
		range = d.range;
		textureX = d.textureX;
		textureY = d.textureY;
	}
};

struct ExtendedCharData {
	uint32_t textureX;
	uint32_t textureY;

	void copy(const ExtendedCharData& d) {
		textureX = d.textureX;
		textureY = d.textureY;
	}
};

extern IntMap<UnitData> unitsData;
extern IntMap<ExtendedCharData> extendedChars;

void loadData(const char *filepath);
