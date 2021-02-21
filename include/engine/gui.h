#pragma once

#include <CXXoverC/dynamicArray.h>
#include <CXXoverC/string.h>
#include <engine/stdint.h>
#include <engine/graphics.h>

#define KEY_STATE_COOLDOWN 5
#define VIRTUAL_KEYBOARD_KEYS 10 + 25 + 1

namespace gui {
	extern image* gui_image;

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

	class TextMenu {
		int32_t selectedEntry = 0;
		uint32_t keyCooldown = 0;

		int32_t drawEntry(uint32_t entryID, uint32_t x, uint32_t y);
	public:
		dynamicArray<string> entries;
		uint32_t getSelectedEntry() { return selectedEntry; }

		int32_t update(uint32_t x, uint32_t y);
	};
}

