#include <engine/graphics.h>
#include <engine/memory.h>
#include <engine/file.h>
#include <stdio.h>

DISPENV disp[2];
DRAWENV draw[2];
int db = 0;

#define PSX_ORDERING_TABLE_SIZE 8
#define PSX_PRIMITIVE_BUFFER_SIZE 32 * 1024

uint32_t ot[2][PSX_ORDERING_TABLE_SIZE];
char pribuff[2][PSX_PRIMITIVE_BUFFER_SIZE];
char* nextpri;

namespace graphics {
	uint32_t Frames = 0;
	uint32_t FPS = 0;
}

void graphics::init() {
	if (*(char*)0xBFC7FF52 != 'E') {
		//NTSC
		SetDefDispEnv(&disp[0], 0, 0, 320, 240);
		SetDefDispEnv(&disp[1], 0, 240, 320, 240);
		SetDefDrawEnv(&draw[0], 0, 240, 320, 240);
		SetDefDrawEnv(&draw[1], 0, 0, 320, 240);
		graphics::FPS = 60;
	}
	else {
		//PAL
		SetDefDispEnv(&disp[0], 0, 0, 320, 256);
		SetDefDispEnv(&disp[1], 0, 256, 320, 256);

		disp[0].screen.y = 24;
		disp[1].screen.y = disp[0].screen.y;

		SetVideoMode(MODE_PAL);

		SetDefDrawEnv(&draw[0], 0, 256, 320, 256);
		SetDefDrawEnv(&draw[1], 0, 0, 320, 256);
		graphics::FPS = 50;
	}

	setRGB0(&draw[0], 63, 0, 127);
	setRGB0(&draw[1], 63, 0, 127);
	draw[0].isbg = 1;
	draw[1].isbg = 1;
	//PutDispEnv(&disp[0]);
	PutDrawEnv(&draw[0]);

	nextpri = pribuff[0];

	FntLoad(960, 0);
	FntOpen(0, 8, 320, 224, 0, 100);
}

image* graphics::loadImage(const char* filepath) {
	uint32_t fileSize = 0;

	uint8_t* fileData = loadFile(filepath, fileSize);
	if (!fileData) {
		return nullptr;
	}

	image* img = new image();

	GetTimInfo((u_int*)fileData, &img->image);

	LoadImage(img->image.prect, (u_int*)img->image.paddr);
	DrawSync(0);

	if (img->image.mode & 0x8) {
		LoadImage(img->image.crect, (u_int*)img->image.caddr);
		DrawSync(0);
	}

	int tim_mode;
	RECT tim_prect, tim_crect;

	tim_prect = *img->image.prect;
	tim_crect = *img->image.crect;
	tim_mode = img->image.mode;

	img->tPage = getTPage(tim_mode & 0x3, 0, tim_prect.x, tim_prect.y);

	img->u_offset = (tim_prect.x % 64) << (2 - (tim_mode & 0x3));
	img->v_offset = (tim_prect.y & 0xff);
	img->width = tim_prect.x << (2 - tim_mode & 0x3);
	img->height = tim_prect.y;
	img->crect = tim_crect;
	img->tim_mode = tim_mode;

	free(fileData);

	return img;
}

void flush() {

}

void graphics::flush_and_display() {
	graphics::Frames++;
	ClearOTagR(ot[db], PSX_ORDERING_TABLE_SIZE);
	flush();

	//printf("%i \n", Frames);
	FntPrint(-1, "HELLO WORLD!");
	FntFlush(-1);

	DrawSync(0);
	VSync(0);
	PutDispEnv(&disp[db]);
	PutDrawEnv(&draw[db]);
	SetDispMask(1);
	DrawOTag(ot[db] + PSX_ORDERING_TABLE_SIZE - 1);
	db = !db;
	nextpri = pribuff[db];
}
