#pragma once

#include <engine/config.h>

#ifdef PLATFORM_DESKTOP
#include <engine/pc/graphics.h>
#endif

#ifdef PLATFORM_PSX
#include <engine/psx/graphics.h>
#endif

namespace graphics {
	void init();
	//void drawSprite(uint32_t imageID, );
}
