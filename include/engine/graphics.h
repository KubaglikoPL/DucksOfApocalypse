#pragma once

#include <engine/config.h>

#ifdef PLATFORM_DESKTOP
#include <engine/pc/graphics.h>
#endif

#ifdef PLATFORM_PSX
#include <engine/psx/graphics.h>
#endif

namespace graphics {
	extern uint32_t Frames;
	extern uint32_t FPS;

	void init();
	image* loadImage(const char* filepath);
	void flush_and_display();
	//void drawSprite(uint32_t imageID, );
}
