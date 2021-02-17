#include <engine/input.h>
#include <psxapi.h>
#include <psxpad.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

u_char padbuff[2][34];
PADTYPE padData;

namespace input {
	uint16_t keyState = 0;
	uint16_t cursorX = 0;
	uint16_t cursorY = 0;
}

void input::init() {
}

void input::update() {
	//prevKeyState = keyState;
	//keyState = 0;

	//printf("%i \n", prevKeyState);

	memcpy((void*)&padData, &padbuff[0], 34);

	//if (padbuff[0][0] == 0)
	//{
	//button = *((u_short*)(padbuff[0] + 2));
	input::keyState = 0;
	if (!(padData.btn & PAD_UP)) input::keyState |= KEY_UP;
	if (!(padData.btn & PAD_DOWN)) input::keyState |= KEY_DOWN;
	if (!(padData.btn & PAD_LEFT)) input::keyState |= KEY_LEFT;
	if (!(padData.btn & PAD_RIGHT)) input::keyState |= KEY_RIGHT;
	if (!(padData.btn & PAD_CROSS)) input::keyState |= KEY_ACTION;
	if (!(padData.btn & PAD_SELECT)) input::keyState |= KEY_SELECT;
	if (!(padData.btn & PAD_START)) input::keyState |= KEY_START;

	//printf("%i \n", input::keyState);
	//if (!(padData.btn & PAD_CIRCLE)) input::keyState |= KEY_BACKSPACE;
	//}

	//printf("X: %i \n", padData.ls_x);
	//printf("Y: %i \n", padData.ls_y);

	int32_t analogX = padData.ls_x - 128;
	int32_t analogY = padData.ls_y - 128;
	//cursorX = padData.ls_x;
	//cursorY = padData.ls_y;
	//float analogX_f, analogY_f;

	//input::cursorX = cursorX_s;
	//input::cursorY = cursorY_s;

	//printf("X: %i \n", analogX);
	//printf("Y: %i \n", analogY);

	if (abs(analogX) > 30) {
		//printf("X: %i \n", analogX);
		input::cursorX = (analogX / 50) + input::cursorX;
		if (input::cursorX < 0) input::cursorX = 0;
		//cursorX_s = cursorX;
	}
	if (abs(analogY) > 30) {
		//printf("Y: %i \n", analogY);
		input::cursorY = (analogY / 50) + input::cursorY;
		if (input::cursorY < 0) input::cursorY = 0;
		//cursorY_s = cursorY;
	}
}

