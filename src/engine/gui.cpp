#include <engine/gui.h>
#include <engine/graphics.h>
#include <engine/input.h>

uint16_t gui::cooldownedKeyState(uint32_t& cooldown) {
#ifndef NON_COOLDOWNED_KEY_STATE
	cooldown++;
	if (cooldown >= KEY_STATE_COOLDOWN) {
		cooldown = 0;
		return input::keyState;
	}
	else {
		return 0;
	}
#else
	return input::keyState;
#endif // !NON_COOLDOWNED_KEY_STATE
}

char gui::VirtualKeyboard::drawCharacter(uint32_t index, char code, uint32_t x, uint32_t y) {
	graphics::drawChar(code, 16 + x * 8, 120 + y * 30);
	if (index == selectedKey) {
		graphics::drawChar('^', 16 + x * 8, (120 + y * 30) + 12);
		return code;
	}
	return 0;
}

char gui::VirtualKeyboard::draw() {
	char return_value = 0;
	for (char c = '0'; c <= '9'; c++) {
		uint8_t i = c - '0';
		char c2 = drawCharacter(i, c, i, 0);
		if (c2 != 0) return_value = c2;
	}
	if (!smallLettersMode) {
		for (char c = 'A'; c <= 'Z'; c++) {
			uint8_t i = c - 'A';
			char c2 = drawCharacter(i + 10, c, i, 1);
			if (c2 != 0) return_value = c2;
		}
	}
	else {
		for (char c = 'a'; c <= 'z'; c++) {
			uint8_t i = c - 'a';
			char c2 = drawCharacter(i + 10, c, i, 1);
			if (c2 != 0) return_value = c2;
		}
	}
	uint8_t z_pos = 'z' - 'a';
	if (drawCharacter(z_pos + 11, '-', z_pos + 2, 1) == '-') {
		return -1;
	}
	return return_value;
}

char gui::VirtualKeyboard::update() {
	char c = draw();
	uint16_t keyState = gui::cooldownedKeyState(keyCooldown);
	if (keyState & KEY_RIGHT) {
		selectedKey++;
		if (selectedKey > VIRTUAL_KEYBOARD_KEYS) {
			selectedKey = 0;
		}
	}
	if (keyState & KEY_LEFT) {
		selectedKey--;
		if (selectedKey < 0) {
			selectedKey = VIRTUAL_KEYBOARD_KEYS;
		}
	}
	if (keyState & KEY_ACTION) {
		return c;
	}
	return 0;
}

int32_t gui::TextMenu::drawEntry(uint32_t entryID, uint32_t x, uint32_t y) {
	string* str = entries.get(entryID);
	if (str) {
		uint32_t stringWidth = graphics::stringWidth(str->data());
		uint32_t entryY = y + (entryID * 14);
		graphics::drawString(str->data(), x - (stringWidth / 2), entryY);
		if (entryID == selectedEntry) {
			graphics::drawChar('>', x - (stringWidth / 2) - 8, entryY);
			graphics::drawChar('<', x + (stringWidth / 2), entryY);
			return entryID;
		}
	}
	return -1;
}

int32_t gui::TextMenu::update(uint32_t x, uint32_t y) {
	int32_t i = -1;
	for (uint32_t ui = 0; ui < entries.getSize(); ui++) {
		int32_t i2 = drawEntry(ui, x, y);
		if (i2 != -1) i = i2;
	}
	uint16_t keyState = gui::cooldownedKeyState(keyCooldown);
	if (keyState & KEY_DOWN) {
		selectedEntry++;
		if (selectedEntry >= entries.getSize()) {
			selectedEntry = 0;
		}
	}
	if (keyState & KEY_UP) {
		selectedEntry--;
		if (selectedEntry < 0) {
			selectedEntry = entries.getSize() - 1;
		}
	}
	if (keyState & KEY_ACTION) {
		return i;
	}
	return -1;
}
