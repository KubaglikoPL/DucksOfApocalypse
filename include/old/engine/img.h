#pragma once

#include <config.h>
#ifdef PLATFORM_PSX
#include <psx/stdint.h>
#include <psxetc.h>
#include <psxgte.h>
#include <psxgpu.h>
#else
#include <stdint.h>
#include <stdbool.h>
#endif // PLATFORM_PSX

#define MAX_IMAGES 32

#ifdef PACKED_GAME_DATA
#define IMAGE_DATA uint8_t*
#endif

#ifdef PLATFORM_DESKTOP
struct image_t {
	int width;
	int height;
	IMAGE_DATA data;
};
#endif
#ifdef PLATFORM_PSX
/*enum imageMode {
	BIT24,
	BIT16,
	BIT8,
	BIT4
}*/

struct image_t {
	TIM_IMAGE image;
	uint32_t tPage;
	RECT crect;
	uint32_t u_offset;
	uint32_t v_offset;
	uint32_t width;
	uint32_t height;
	int tim_mode;
};
#endif // PLATFORM_PSX

//typedef struct image image_t;

extern image_t images[MAX_IMAGES];

#define IMAGE_LOAD_SUCCESS 1
#define IMAGE_LOAD_NO_FILE 2
#define IMAGE_LOAD_INVALID_FORMAT 3
#define IMAGE_LOAD_INVALID_IMAGE 4

int load_image(const char *filename, unsigned int image);

//Predefined Image IDs
#define IMAGE_FONT 1
#define IMAGE_GUI 2
#define IMAGE_SPRITE_SHEET0 3
#define IMAGE_TERRAIN 10
