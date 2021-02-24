#include <engine/gui.h>
#include <engine/graphics.h>
#include <engine/input.h>
#include <engine/text.h>

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

namespace gui {
	image* gui_image;
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
	if (keyState & KEY_MENU_ACTION) {
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
		if (selectedEntry >= static_cast<int32_t>(entries.getSize())) {
			selectedEntry = 0;
		}
	}
	if (keyState & KEY_UP) {
		selectedEntry--;
		if (selectedEntry < 0) {
			selectedEntry = entries.getSize() - 1;
		}
	}
	if (keyState & KEY_MENU_ACTION) {
		return i;
	}
	return -1;
}

namespace gui {
	gui::GameGUI activeGUI = gui::GameGUI::MAIN_MENU;
	bool ingame = false;
	bool paused = false;
}

gui::TextMenu mainMenuTextMenu;
bool mainMenuSetuped = false;

void setupMainMenu() {
	mainMenuTextMenu.entries.add((*text::getText(TEXT_GUI_NEW_GAME)));
	mainMenuTextMenu.entries.add((*text::getText(TEXT_GUI_LOAD_GAME)));
#ifdef MAIN_MENU_HAS_SETTINGS_AND_EXIT
	mainMenuTextMenu.entries.add((*text::getText(TEXT_GUI_OPTIONS)));
	mainMenuTextMenu.entries.add((*text::getText(TEXT_GUI_EXIT)));
#endif
	mainMenuSetuped = true;
}

void mainMenuUpdate() {
	int32_t selected = mainMenuTextMenu.update(graphics::screenWidth / 2, 40);
	graphics::drawStringCentered((*text::getText(TEXT_GUI_TITLE)), graphics::screenWidth / 2, 10);
	graphics::drawString((*text::getText(TEXT_GUI_CREDITS)), 8, graphics::screenHeight - 30);
	graphics::drawString((*text::getText(TEXT_GUI_VERSION)), 8, graphics::screenHeight - 15);
	if (selected == 0) {
		gui::activeGUI = gui::GameGUI::NO_GUI;
		gui::ingame = true;
		gui::paused = false;
	}
#ifdef MAIN_MENU_HAS_SETTINGS_AND_EXIT
	if (selected == 3) {
		exit(0);
	}
#endif
}

void gui::guiMain() {
	if (activeGUI == gui::GameGUI::MAIN_MENU) {
		if (!mainMenuSetuped) setupMainMenu();
		mainMenuUpdate();
	}
}
