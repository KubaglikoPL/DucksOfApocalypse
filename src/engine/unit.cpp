#include <engine/unit.h>
#include <engine/gui.h>
#include <engine/input.h>

dynamicArray<Unit> units;
image* units_image = nullptr;

void drawUnits() {
	if (units_image) {
		for (uint32_t i = 0; i < units.getSize(); i++) {
			Unit* unit = units.get(i);
			uint32_t tx_col = (unit->type - 1) % 16;
			uint32_t tx_row = (unit->type - 1) / 16;
			graphics::drawSprite(units_image, 16, 16, tx_col * 16, tx_row * 16, unit->x, unit->y);
			if (unit->selected) {
				graphics::drawSprite(gui::gui_image, 16, 16, 0, 16, unit->x, unit->y);
			}
		}
	}
}

void updateUnits() {
	if (input::keyState & KEY_ACTION) {
		for (uint32_t i = 0; i < units.getSize(); i++) {
			Unit* unit = units.get(i);
			uint32_t unit_tile_x = unit->x / 16;
			uint32_t unit_tile_y = unit->y / 16;
			uint32_t cursor_tile_x = input::cursorX / 16;
			uint32_t cursor_tile_y = input::cursorY / 16;

			if ((unit_tile_x == cursor_tile_x) && (unit_tile_y == cursor_tile_y)) {
				unit->selected = !unit->selected;
			}
			else {
				if (unit->selected) {
					unit->x = cursor_tile_x * 16;
					unit->y = cursor_tile_y * 16;
					unit->selected = false;
				}
			}
		}
	}
	else {
		uint32_t cursor_tile_x = input::cursorX / 16;
		uint32_t cursor_tile_y = input::cursorY / 16;
		graphics::drawSprite(gui::gui_image, 16, 16, 16, 16, cursor_tile_x * 16, cursor_tile_y * 16);
	}
}
