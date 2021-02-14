#pragma once

#include <engine/stdint.h>
#include <psxetc.h>
#include <psxgte.h>
#include <psxgpu.h>

struct image {
	TIM_IMAGE img;
	uint32_t tPage;
	RECT crect;
	uint32_t u_offset;
	uint32_t v_offset;
	uint32_t width;
	uint32_t height;
	int tim_mode;
};
