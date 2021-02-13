#include <engine/graphics.h>

DISPENV disp[2];
DRAWENV draw[2];
int db = 0;

void graphics::init() {
	if (*(char*)0xBFC7FF52 != 'E') {
		//NTSC
		SetDefDispEnv(&disp[0], 0, 0, 320, 240);
		SetDefDispEnv(&disp[1], 0, 240, 320, 240);
		SetDefDrawEnv(&draw[0], 0, 240, 320, 240);
		SetDefDrawEnv(&draw[1], 0, 0, 320, 240);
		//FPS = 60;
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
		//FPS = 50;
	}

	setRGB0(&draw[0], 63, 0, 127);
	setRGB0(&draw[1], 63, 0, 127);
	draw[0].isbg = 1;
	draw[1].isbg = 1;
	//PutDispEnv(&disp[0]);
	PutDrawEnv(&draw[0]);

	//nextpri = pribuff[0];

	FntLoad(960, 0);
	FntOpen(0, 8, 320, 224, 0, 100);
}
