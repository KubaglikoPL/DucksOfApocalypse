#include <stdio.h>
#include <engine/stdint.h>
#include <engine/memory.h>
#include <engine/file.h>
#include <psxcd.h>

uint8_t* loadFile(const char* filepath, uint32_t& fileSize) {
	CdlFILE file;

	if (!CdSearchFile(&file, filepath))
	{
		printf("File not found. %s \n", filepath);
		return nullptr;
	}

	printf("File found! %s \n", filepath);

	fileSize = 0;
	uint8_t* fileData;

	fileSize = 2048 * ((file.size + 2047) / 2048);
	fileData = (uint8_t*)malloc(fileSize);
	CdControl(CdlSetloc, (u_char*)&file.pos, 0);
	CdRead((file.size + 2047) / 2048, (unsigned int*)fileData, CdlModeSpeed);
	CdReadSync(0, 0);
	return fileData;
}
