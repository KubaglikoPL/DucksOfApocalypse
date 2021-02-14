#include <stdio.h>
#include <engine/stdint.h>
#include <engine/memory.h>
#include <engine/file.h>

uint8_t* loadFile(const char* filepath, uint32_t& fileSize) {
	FILE* file;
	file = fopen(filepath, "rb");
	if (file) {
		fseek(file, 0, SEEK_END);
		fileSize = ftell(file);
		fseek(file, 0, SEEK_SET);

		uint8_t* fileBuffer = (uint8_t*)malloc(fileSize);

		int i = fread(fileBuffer, 1, fileSize, file);
		return fileBuffer;
	}
	else {
		printf("File not found \n");
		return nullptr;
	}
}
