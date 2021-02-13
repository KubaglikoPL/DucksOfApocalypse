#include <engine/file_loader_psx.h>

#ifdef PLATFORM_PSX
#include <psxcd.h>
#include <malloc.h>
#include <stdio.h>

uint8_t* loadFileFromCD(const char* filepath, uint32_t* size) {
	CdlFILE file;

	if (!CdSearchFile(&file, filepath))
	{
		printf("File not found. %s \n", filepath);
		return nullptr;
	}

	printf("File found! %s \n", filepath);

	(*size) = 0;
	uint8_t* fileData;

	(*size) = 2048 * ((file.size + 2047) / 2048);
	fileData = (uint8_t*)malloc((*size));
	CdControl(CdlSetloc, (u_char*)&file.pos, 0);
	CdRead((file.size + 2047) / 2048, (unsigned int*)fileData, CdlModeSpeed);
	CdReadSync(0, 0);
	return fileData;
}
#endif