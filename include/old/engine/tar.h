#pragma once

#include <config.h>
#ifdef PLATFORM_PSX
	#include <psx/stdint.h>
#else
	#include <stdint.h>
	#include <stdbool.h>
#endif // PLATFORM_PSX

struct tar_entry {
	const char* filename;
	uint32_t size;
	uint32_t padded_size;
	uint32_t start_offset;
};

typedef struct tar_entry tar_entry_t;

bool load_tar_file(const char *filename);
uint8_t* load_tar_entry(const char* filename, int *size);
