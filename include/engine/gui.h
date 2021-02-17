#pragma once

#include <engine/stdint.h>
#include <engine/graphics.h>

#define KEY_STATE_COOLDOWN 5
#define VIRTUAL_KEYBOARD_KEYS 10 + 25 + 1

namespace gui {
	uint16_t cooldownedKeyState(uint32_t& cooldown);

	class VirtualKeyboard {
		int32_t selectedKey = 0;
		uint32_t keyCooldown = 0;
		bool smallLettersMode = false;

		char drawCharacter(uint32_t index, char code, uint32_t x, uint32_t y);
		char draw();
	public:
		char update();
	};
}

