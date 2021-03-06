#pragma once

#include <engine/stdint.h>
#include <CXXoverC/dynamicArray.h>
#include <engine/graphics.h>
#include <engine/data.h>

#define UNIT_TYPE_DUCK 1

extern image* units_image;

class Unit {
public:
	uint8_t type = 0;
	uint8_t hp = 0;
	uint8_t move = 0;
	uint16_t x = 0;
	uint16_t y = 0;
	UnitData* data = NULL;
	bool selected = false;

	void copy(const Unit& unit) {
		type = unit.type;
		hp = unit.hp;
		move = unit.move;
		x = unit.x;
		y = unit.y;
		selected = unit.selected;
		data = unit.data;
	}
};

extern dynamicArray<Unit> units;

void drawUnits();
void updateUnits();
