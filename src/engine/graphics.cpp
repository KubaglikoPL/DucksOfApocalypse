#include <engine/graphics.h>
#include <engine/text.h>
#include <stdio.h>

namespace graphics {
	dynamicArray<SpriteInstance> spriteInstances;
}

void graphics::clearSprites() {
	graphics::spriteInstances.clear();
}

void graphics::drawSprite(image* img, uint32_t x, uint32_t y) {
	drawSprite(img, img->width, img->height, x, y);
}

void graphics::drawSprite(image* img, uint32_t width, uint32_t height, uint32_t x, uint32_t y) {
	drawSprite(img, width, height, 0, 0, x, y);
}

void graphics::drawSprite(image* img, uint32_t width, uint32_t height, uint32_t u_offset, uint32_t v_offset, uint32_t x, uint32_t y) {
	//printf("B \n");
	SpriteInstance i;
	i.img = img;
	i.width = width;
	i.height = height;
	i.u_offset = u_offset;
	i.v_offset = v_offset;
	i.x = x;
	i.y = y;
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

#pragma optimize("", off)
void graphics::drawChar(uint16_t unicodeCode, uint32_t x, uint32_t y) {
	if (unicodeCode < 128) {
		volatile uint32_t charCode = unicodeCode - 32;
		volatile uint32_t tx_row = charCode / 32;
		volatile uint32_t tx_col = charCode % 32;
		graphics::drawSprite(graphics::font, 8, 12, tx_col * 8, tx_row * 12, x, y);
	}
	else {
		//TODO Extended chars
	}
}
#pragma optimize("", on)