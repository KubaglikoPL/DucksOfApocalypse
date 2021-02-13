#pragma once

#include <config.h>
#ifdef PLATFORM_PSX
#include <psx/stdint.h>
uint8_t* loadFileFromCD(const char* filepath, uint32_t* size);
#endif