#include <engine/tar.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

tar_entry_t* tar_entries = NULL;
int tar_entries_number = 0;

#ifndef PLATFORM_PSX
FILE* file = NULL;
uint8_t tar_header[512];

int tar_load_entries(tar_entry_t* entries) {
	fseek(file, 0, SEEK_SET);
	int i = 0;
	while (!feof(file)) {
		fread(&tar_header[0], 512, 1, file);
		if (tar_header[0] == 0) break;
		tar_entry_t entry;
		int filename_size = strlen((const char *)&tar_header[0]);
		entry.filename = (const char *)malloc(filename_size + 1);
		memcpy((void*)entry.filename, &tar_header[0], filename_size + 1);
		entry.size = strtol((const char*)&tar_header[124], NULL, 8);
		entry.padded_size = entry.size + (512 - (entry.size % 512));
		entry.start_offset = ftell(file);
		if (entries) {
			entries[i] = entry;
		}
		fseek(file, entry.padded_size, SEEK_CUR);
		i++;
	}

	return i;
}

bool load_tar_file(const char* filename) {
	file = fopen(filename, "r");
	if (file) {
		tar_entries_number = tar_load_entries(NULL);
		tar_entries = (tar_entry_t*)malloc(sizeof(tar_entry_t) * tar_entries_number);
		tar_load_entries(tar_entries);
		return true;
	}
	else {
		return false;
	}
}

uint8_t* load_tar_entry(const char* filename, int* size) {
	for (int i = 0; i < tar_entries_number; i++) {
		if (!strcmp(filename, tar_entries[i].filename)) {
			fseek(file, tar_entries[i].start_offset, SEEK_SET);
			uint8_t* fileData = (uint8_t*)malloc(tar_entries[i].size);
			fread(fileData, tar_entries[i].size, 1, file);
			(*size) = tar_entries[i].size;
			return fileData;
		}
	}

	return NULL;
}
#endif
#ifdef PSX_BUILD
	/*#include <psxcd.h>

	CdlFILE file;

	uint8_t* tarFileBuffer;
	uint32_t tarFileSize;

	int tar_load_entries(tar_entry_t* entries) {
		//fseek(file, 0, SEEK_SET);
		int i = 0;
		while (i < tarFileSize) {
			//fread(&tar_header[0], 512, 1, file);
			if (tarFileBuffer[0] == 0) break;
			tar_entry_t entry;
			int filename_size = strlen((const char*)&tarFileBuffer[i]);
			entry.filename = (const char*)malloc(filename_size + 1);
			memcpy((void*)entry.filename, &tarFileBuffer[i], filename_size + 1);
			entry.size = strtol((const char*)&tarFileBuffer[i + 124], NULL, 8);
			entry.padded_size = entry.size + (512 - (entry.size % 512));
			entry.start_offset = i + 512;
			if (entries) {
				entries[i] = entry;
			}
			//fseek(file, entry.padded_size, SEEK_CUR);
			i = i + entry.padded_size;
			i++;
		}

		return i;
	}

	bool load_tar_file(const char* filename) {
		if (!CdSearchFile(&file, "\\DATA.TAR;1"))
		{
			// Return value is NULL, file is not found
			printf("File not found.\n");
			return false;
		}

		printf("File found!\n");
		tarFileSize = 2048 * ((file.size + 2047) / 2048);
		tarFileBuffer = (uint8_t*)malloc(tarFileSize);
		CdControl(CdlSetloc, (u_char*)&file.pos, 0);
		CdRead((file.size + 2047) / 2048, (unsigned int*)tarFileBuffer, CdlModeSpeed);
		CdReadSync(0, 0);
		tar_entries_number = tar_load_entries(NULL);
		tar_entries = (tar_entry_t*)malloc(sizeof(tar_entry_t) * tar_entries_number);
		tar_load_entries(tar_entries);
		return true;
	}

	uint8_t* load_tar_entry(const char* filename, int* size) {
		printf("%i \n", tar_entries_number);
		for (int i = 0; i < tar_entries_number; i++) {
			printf("%s \n", tar_entries[i].filename);
			if (!strcmp(filename, tar_entries[i].filename)) {
				//fseek(file, tar_entries[i].start_offset, SEEK_SET);
				uint8_t* fileData = (uint8_t*)malloc(tar_entries[i].size);
				//fread(fileData, tar_entries[i].size, 1, file);
				memcpy(fileData, (const void*)tarFileBuffer[tar_entries[i].start_offset], tar_entries[i].size);
				(*size) = tar_entries[i].size;
				return fileData;
			}
		}

		return NULL;
	}*/
#endif // PSX_BUILD

