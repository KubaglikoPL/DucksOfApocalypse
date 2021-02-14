#include <engine/graphics.h>
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
