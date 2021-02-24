#pragma once

#include <engine/stdint.h>

#define KEY_UP 1
#define KEY_DOWN 2
#define KEY_LEFT 4
#define KEY_RIGHT 8
#define KEY_START 16 //Escape on PC
#define KEY_SELECT 32 //E on PC
#define KEY_ACTION 64 //Left Mouse Button on PC
#define KEY_MENU_ACTION 128 //Enter on PC

namespace input {
	void init();
	void update();

	extern uint16_t keyState;
	extern uint16_t cursorX;
	extern uint16_t cursorY;
}
