#include <engine/render.h>
#include <engine/img.h>
#include <engine/text.h>
#ifdef PLATFORM_PSX
#include <psx/stdint.h>
#include <psxetc.h>
#include <psxgte.h>
#include <psxgpu.h>
#include <stdio.h>

DISPENV disp[2];
DRAWENV draw[2];
int db = 0;

uint32_t ot[2][PSX_ORDERING_TABLE_SIZE];
char pribuff[2][32768];
char* nextpri;

void init_rendering() {
	ResetGraph(0);

	//Currently fot NTSC, TODO(NTSC/PAL Detection)

	if (*(char*)0xBFC7FF52 != 'E') {
		//NTSC
		SetDefDispEnv(&disp[0], 0, 0, 320, 240);
		SetDefDispEnv(&disp[1], 0, 240, 320, 240);
		SetDefDrawEnv(&draw[0], 0, 240, 320, 240);
		SetDefDrawEnv(&draw[1], 0, 0, 320, 240);
		FPS = 60;
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
		FPS = 50;
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

int activeTexPage = -1999;

void clear_screen() {
	ClearOTagR(ot[db], PSX_ORDERING_TABLE_SIZE);
	activeTexPage = -1999;
}

void display_screen() {
	Frames++;
	//printf("%i \n", Frames);
	//FntPrint(-1, "HELLO WORLD!");
	//FntFlush(-1);

	DrawSync(0);
	VSync(0);
	PutDispEnv(&disp[db]);
	PutDrawEnv(&draw[db]);
	SetDispMask(1);
	DrawOTag(ot[db] + PSX_ORDERING_TABLE_SIZE - 1);
	db = !db;
	nextpri = pribuff[db];
}

void draw_sprite_imageID(int imageID, int x, int y) {
	image_t* img = &images[imageID];
	draw_sprite_tx_offset(img, img->width, img->height, 0, 0, x, y);
}

void draw_sprite_tx_offset_imageID(int imageID, int width, int height, int tx_offset_x, int tx_offset_y, int x, int y) {
	image_t* img = &images[imageID];
	draw_sprite_tx_offset(img, width, height, tx_offset_x, tx_offset_y, x, y);
}

void draw_sprite_tx_offset(image_t* image, int width, int height, int tx_offset_x, int tx_offset_y, int x, int y) {
	SPRT* sprt = (SPRT*)nextpri;

	setSprt(sprt);
	setXY0(sprt, cameraX + x, cameraY + y);
	setWH(sprt, width, height);
	setUV0(sprt, image->u_offset + tx_offset_x, image->v_offset + tx_offset_y);
	setClut(sprt, image->crect.x, image->crect.y);
	setRGB0(sprt, 128, 128, 128);
	addPrim(ot[db], sprt);

	nextpri += sizeof(SPRT);

	//printf("%i \n", image->tPage);
	//if (image->tPage != activeTexPage) {
		/*DR_TPAGE* tpage;
		tpage = (DR_TPAGE*)nextpri;
		setDrawTPage(tpage, 0, 1, image->tPage);
		addPrim(ot[db], tpage);
		activeTexPage = image->tPage;
		nextpri += sizeof(DR_TPAGE);*/
	//}
}

void set_image(int imageID) {
	set_image(&images[imageID]);
}

void set_image(image_t* image) {
	DR_TPAGE* tpage;
	tpage = (DR_TPAGE*)nextpri;
	setDrawTPage(tpage, 0, 1, image->tPage);
	addPrim(ot[db], tpage);
	activeTexPage = image->tPage;
	nextpri += sizeof(DR_TPAGE);
}

#endif 