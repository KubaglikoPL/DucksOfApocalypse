#pragma once

#include <engine/config.h>
#include <CXXoverC/dynamicArray.h>
#include <CXXoverC/string.h>

#ifdef PLATFORM_DESKTOP
#include <engine/pc/graphics.h>
#endif

#ifdef PLATFORM_PSX
#include <engine/psx/graphics.h>
#endif

#define GRAPHICS_MAX_SPRITES

namespace graphics {
	extern uint32_t Frames;
	extern uint32_t FPS;
	extern uint32_t screenWidth;
	extern uint32_t screenHeight;

	class SpriteInstance {
	public:
		image* img;
		uint32_t width;
		uint32_t height;
		uint32_t u_offset;
		uint32_t v_offset;
		uint32_t x;
		uint32_t y;
		bool trasparency;

		void copy(const SpriteInstance& i) {
			img = i.img;
			width = i.width;
			height = i.height;
			u_offset = i.u_offset;
			v_offset = i.v_offset;
			x = i.x;
			y = i.y;
			trasparency = i.trasparency;
		}
	};

	extern dynamicArray<SpriteInstance> spriteInstances;
	extern image* font;

	//Platform Specific
	void init();
	image* loadImage(const char* filepath);
	void flush_and_display();

	//Platform Independent
	void clearSprites();
	void drawSprite(image *img, uint32_t x, uint32_t y, bool trasparency = true);
	void drawSprite(image* img, uint32_t width, uint32_t height, uint32_t x, uint32_t y, bool trasparency = true);
	void drawSprite(image* img, uint32_t width, uint32_t height, uint32_t u_offset, uint32_t v_offset, uint32_t x, uint32_t y, bool trasparency = true);
	
	void drawString(string str, uint32_t x, uint32_t y);
	void drawString(const char* str, uint32_t x, uint32_t y);
	void drawChar(char c, uint32_t x, uint32_t y);
	void drawChar(uint16_t unicodeCode, uint32_t x, uint32_t y);

	uint32_t stringWidth(const char* str);
	void drawStringCentered(string str, uint32_t x, uint32_t y);
	void drawStringCentered(const char* str, uint32_t x, uint32_t y);
}
