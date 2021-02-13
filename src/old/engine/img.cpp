#include <engine/img.h>
#include <engine/tar.h>
#ifdef PLATFORM_DESKTOP
#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_HDR
#define STBI_ONLY_TGA
#define STBI_NO_STDIO
#define STBI_NO_LINEAR
#define STBI_WINDOWS_UTF8
#include <stb_image.h>
#include <stdio.h>
#endif

image_t images[MAX_IMAGES];

#ifdef PLATFORM_DESKTOP
int load_image(const char* filename, unsigned int image) {
#ifdef PACKED_GAME_DATA
	if (image >= MAX_IMAGES) {
		return IMAGE_LOAD_INVALID_IMAGE;
	}

	int fileSize = 0;
	uint8_t* fileData = load_tar_entry(filename, &fileSize);
	if(!fileData) return IMAGE_LOAD_NO_FILE;

	int w = 0, h = 0, comp = 0;

	uint8_t* imageData = stbi_load_from_memory(fileData, fileSize, &w, &h, &comp, 3);
	free(fileData);
	if ((w == 0) || (h == 0) || (comp == 0) || (imageData == NULL)) {
		return IMAGE_LOAD_INVALID_FORMAT;
	}

	images[image].data = imageData;
	images[image].width = w;
	images[image].height = h;
	return IMAGE_LOAD_SUCCESS;
#endif
}
#endif

#ifdef PLATFORM_PSX
#include <malloc.h>
#include <stdio.h>
#include <psxcd.h>
#include <engine/file_loader_psx.h>

int load_image(const char* filename, unsigned int image) {
	if (image >= MAX_IMAGES) {
		return IMAGE_LOAD_INVALID_IMAGE;
	}

	uint32_t fileSize = 0;
	uint8_t* fileData;

	fileData = loadFileFromCD(filename, &fileSize);

	GetTimInfo((u_int*)fileData, &images[image].image);

	LoadImage(images[image].image.prect, (u_int*)images[image].image.paddr);
	DrawSync(0);

	if (images[image].image.mode & 0x8) {
		LoadImage(images[image].image.crect, (u_int*)images[image].image.caddr);
		DrawSync(0);
	}

	int tim_mode;
	RECT tim_prect, tim_crect;

	tim_prect = *images[image].image.prect;
	tim_crect = *images[image].image.crect;
	tim_mode = images[image].image.mode;

	images[image].tPage = getTPage(tim_mode & 0x3, 0, tim_prect.x, tim_prect.y);

	images[image].u_offset = (tim_prect.x % 64) << (2 - (tim_mode & 0x3));
	images[image].v_offset = (tim_prect.y & 0xff);
	images[image].width = tim_prect.x << (2 - tim_mode & 0x3);
	images[image].height = tim_prect.y;
	images[image].crect = tim_crect;
	images[image].tim_mode = tim_mode;

	free(fileData);
}
#endif
