#include <engine/graphics.h>
#include <engine/text.h>
#include <engine/data.h>
#include <stdio.h>

namespace graphics {
	dynamicArray<SpriteInstance> spriteInstances;
}

void graphics::clearSprites() {
	graphics::spriteInstances.clear();
}

void graphics::drawSprite(image* img, uint32_t x, uint32_t y, bool trasparency) {
	drawSprite(img, img->width, img->height, x, y, trasparency);
}

void graphics::drawSprite(image* img, uint32_t width, uint32_t height, uint32_t x, uint32_t y, bool trasparency) {
	drawSprite(img, width, height, 0, 0, x, y, trasparency);
}

void graphics::drawSprite(image* img, uint32_t width, uint32_t height, uint32_t u_offset, uint32_t v_offset, uint32_t x, uint32_t y, bool trasparency) {
	//printf("B \n");
	SpriteInstance i;
	i.img = img;
	i.width = width;
	i.height = height;
	i.u_offset = u_offset;
	i.v_offset = v_offset;
	i.x = x;
	i.y = y;
	i.trasparency = trasparency;
	graphics::spriteInstances.add(i);
}

void graphics::drawString(string str, uint32_t x, uint32_t y) {
	drawString(str.data(), x, y);
}

void graphics::drawString(const char* str, uint32_t x, uint32_t y) {
	uint32_t i = 0;
	uint32_t x_offset = 0;
	uint16_t unicodePoint = 0;
	while (str[i]) {
		i += text::utf8Convert(&unicodePoint, str, i);
		drawChar(unicodePoint, x + x_offset, y);
		x_offset += 8;
	}
}

void graphics::drawChar(char c, uint32_t x, uint32_t y) {
	drawChar(static_cast<uint16_t>(c), x, y);
}

uint32_t graphics::stringWidth(const char* str) {
	uint32_t i = 0;
	uint32_t x_offset = 0;
	uint16_t unicodePoint = 0;
	while (str[i]) {
		i += text::utf8Convert(&unicodePoint, str, i);
		x_offset += 8;
	}
	return x_offset;
}

void graphics::drawStringCentered(string str, uint32_t x, uint32_t y) {
	drawStringCentered(str.data(), x, y);
}

void graphics::drawStringCentered(const char* str, uint32_t x, uint32_t y) {
	graphics::drawString(str, x - (graphics::stringWidth(str) / 2), y);
}

#pragma optimize("", off)
void graphics::drawChar(uint16_t unicodeCode, uint32_t x, uint32_t y) {
	if (unicodeCode < 128) {
		volatile uint32_t charCode = unicodeCode - 32;
		volatile uint32_t tx_row = charCode / 32;
		volatile uint32_t tx_col = charCode % 32;
		graphics::drawSprite(graphics::font, 8, 12, tx_col * 8, tx_row * 12, x, y);
	}
	else {
		ExtendedCharData* d = extendedChars.get(unicodeCode);
		if (d) {
			volatile uint32_t tx_row = d->textureY;
			volatile uint32_t tx_col = d->textureX;
			graphics::drawSprite(graphics::font, 8, 12, tx_col * 8, tx_row * 12, x, y);
		}
		else {
			drawChar('?', x, y);
		}
	}
}
#pragma optimize("", on)