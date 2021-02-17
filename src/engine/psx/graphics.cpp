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
	uint32_t screenWidth = 320;
	uint32_t screenHeight = 240;
	image* font = nullptr;
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
		screenHeight = 256;

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

	GetTimInfo((u_int*)fileData, &img->img);

	LoadImage(img->img.prect, (u_int*)img->img.paddr);
	DrawSync(0);

	if (img->img.mode & 0x8) {
		LoadImage(img->img.crect, (u_int*)img->img.caddr);
		DrawSync(0);
	}

	int tim_mode;
	RECT tim_prect, tim_crect;

	tim_prect = *img->img.prect;
	tim_crect = *img->img.crect;
	tim_mode = img->img.mode;

	img->tPage = getTPage(tim_mode & 0x3, 0, tim_prect.x, tim_prect.y);

	img->u_offset = (tim_prect.x % 64) << (2 - (tim_mode & 0x3));
	img->v_offset = (tim_prect.y & 0xff);
	img->width = tim_prect.w << (2 - tim_mode & 0x3);
	img->height = tim_prect.h;
	img->crect = tim_crect;
	img->tim_mode = tim_mode;

	free(fileData);

	return img;
}

//image* activeImage = nullptr;

void drawSpriteInstance(graphics::SpriteInstance* instance) {
	if (instance) {
		SPRT* sprt = (SPRT*)nextpri;
		setSprt(sprt);
		setXY0(sprt, instance->x, instance->y);
		setWH(sprt, instance->width, instance->height);
		setUV0(sprt, instance->img->u_offset + instance->u_offset, instance->img->v_offset + instance->v_offset);
		setClut(sprt, instance->img->crect.x, instance->img->crect.y);
		setRGB0(sprt, 128, 128, 128);
		addPrim(ot[db], sprt);

		nextpri += sizeof(SPRT);

		//if (instance->img != activeImage) {
			//printf("C\n");
			DR_TPAGE* tpage;
			tpage = (DR_TPAGE*)nextpri;
			setDrawTPage(tpage, 0, 1, instance->img->tPage);
			addPrim(ot[db], tpage);
			//activeImage = instance->img;
			nextpri += sizeof(DR_TPAGE);
		//}
	}
}

void flush() {
	//activeImage = nullptr;
	for (uint32_t i = 0; i < graphics::spriteInstances.getSize(); i++) {
		graphics::SpriteInstance* instance = graphics::spriteInstances.get(((graphics::spriteInstances.getSize() - i)) - 1);

		drawSpriteInstance(instance);
	}
}

void graphics::flush_and_display() {
	graphics::Frames++;
	ClearOTagR(ot[db], PSX_ORDERING_TABLE_SIZE);
	flush();

	DrawSync(0);
	VSync(0);
	PutDispEnv(&disp[db]);
	PutDrawEnv(&draw[db]);
	SetDispMask(1);
	DrawOTag(ot[db] + PSX_ORDERING_TABLE_SIZE - 1);
	db = !db;
	nextpri = pribuff[db];
}
