#include <engine/graphics.h>
#include <engine/memory.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

uint8_t* screenBuffer;

void graphics::init() {
#ifdef RGB24_SCREEN_BUFFER
	screenBuffer = (uint8_t*)malloc(SCREEN_WIDTH * SCREEN_HEIGHT * 3);
#endif
}
