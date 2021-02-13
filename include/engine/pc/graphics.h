#pragma once

#include <engine/stdint.h>

struct image {
	uint32_t width;
	uint32_t height;
	uint8_t* data;
};
