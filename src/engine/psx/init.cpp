#include <engine/engine.h>
#include <engine/psx/graphics.h>
#include <psxcd.h>
#include <psxapi.h>

extern u_char padbuff[2][34];

void init() {
	InitPAD((char*)&padbuff[0][0], 34, (char*)&padbuff[1][0], 34);
	StartPAD();
	ChangeClearPAD(1);
	ResetGraph(0);
	CdInit();
}

bool isGameRunning() {
	return true;
}
