#include <engine/gui.h>
#include <engine/graphics.h>
#include <engine/input.h>
#include <engine/text.h>
#include <engine/unit.h>

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

void gui::guiBox(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
	uint32_t w = width - 1;
	uint32_t h = height - 1;
	graphics::drawSprite(gui::gui_image, 8, 8, 32, 0, x, y);
	graphics::drawSprite(gui::gui_image, 8, 8, 40, 0, x + w * 8, y);
	graphics::drawSprite(gui::gui_image, 8, 8, 32, 8, x , y + h * 8);
	graphics::drawSprite(gui::gui_image, 8, 8, 40, 8, x + w * 8, y + h * 8);
	for (uint32_t i = 0; i < (width - 2); i++) {
		graphics::drawSprite(gui::gui_image, 8, 8, 48, 0, x + (i + 1) * 8, y);
		graphics::drawSprite(gui::gui_image, 8, 8, 56, 8, x + (i + 1) * 8, y + h * 8);
	}
	for (uint32_t i = 0; i < (height - 2); i++) {
		graphics::drawSprite(gui::gui_image, 8, 8, 48, 8, x, y + (i + 1) * 8);
		graphics::drawSprite(gui::gui_image, 8, 8, 56, 0, x + w * 8, y + (i + 1) * 8);
	}
	for (uint32_t yy = 0; yy < (height - 2); yy++) {
		for (uint32_t xx = 0; xx < (width - 2); xx++) {
			graphics::drawSprite(gui::gui_image, 8, 8, 56, 16, x + (xx + 1) * 8, y + (yy + 1) * 8);
		}
	}
}

namespace gui {
	//gui::GameGUI activeGUI = gui::GameGUI::MAIN_MENU;
	gui::GameGUI activeGUI = gui::GameGUI::INGAME_GUI;
	gui::GameSubGUI activeSubGUI = gui::GameSubGUI::NO_GUI;
	//bool ingame = false;
	bool ingame = true;
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
	//gui::guiBox(0, 0, 5, 5);
	if (selected == 0) {
		gui::activeGUI = gui::GameGUI::INGAME_GUI;
		gui::ingame = true;
		gui::paused = false;
	}
#ifdef MAIN_MENU_HAS_SETTINGS_AND_EXIT
	if (selected == 3) {
		exit(0);
	}
#endif
}

string selectedUnitGUI_hp;
string selectedUnitGUI_attack;
string selectedUnitGUI_armor;
string selectedUnitGUI_move;

extern Unit* selectedUnit;

void selectedUnitGUI() {
	uint32_t boxY = graphics::screenHeight - 88;
	gui::guiBox(0, boxY, 20, 11);
	graphics::drawSprite(units_image, 16, 16, selectedUnit->data->textureX * 16, selectedUnit->data->textureY * 16, 1, boxY + 1);
	graphics::drawString(*text::getText(selectedUnit->data->textID), 4, boxY + 18);
	
	selectedUnitGUI_hp.clear();
	selectedUnitGUI_hp.add("HP: ");
	selectedUnitGUI_hp.add(selectedUnit->hp);
	selectedUnitGUI_hp.add("/");
	selectedUnitGUI_hp.add(selectedUnit->data->maxHP);
	graphics::drawString(selectedUnitGUI_hp, 4, boxY + 18 + 13);

	selectedUnitGUI_attack.clear();
	selectedUnitGUI_attack.add("Attack: ");
	selectedUnitGUI_attack.add(selectedUnit->data->minAttack);
	selectedUnitGUI_attack.add("-");
	selectedUnitGUI_attack.add(selectedUnit->data->maxAttack);
	graphics::drawString(selectedUnitGUI_attack, 4, boxY + 18 + 13 + 13);

	selectedUnitGUI_armor.clear();
	selectedUnitGUI_armor.add("Armor: ");
	selectedUnitGUI_armor.add(selectedUnit->data->armor);
	graphics::drawString(selectedUnitGUI_armor, 4, boxY + 18 + 13 + 13 + 13);

	selectedUnitGUI_move.clear();
	selectedUnitGUI_move.add("Move: ");
	selectedUnitGUI_move.add(selectedUnit->move);
	selectedUnitGUI_move.add("/");
	selectedUnitGUI_move.add(selectedUnit->data->move);
	graphics::drawString(selectedUnitGUI_move, 4, boxY + 18 + 13 + 13 + 13 + 13);
}

void gui::guiMain() {
	if (activeGUI == gui::GameGUI::MAIN_MENU) {
		if (!mainMenuSetuped) setupMainMenu();
		mainMenuUpdate();
	}
	else if (activeGUI == gui::GameGUI::INGAME_GUI) {
		//if (activeSubGUI == gui::GameSubGUI::SELECTED_UNIT_GUI) {
		if (selectedUnit != nullptr) {
			selectedUnitGUI();
		}
		//}
	}
}
