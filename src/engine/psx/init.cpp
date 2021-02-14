#include <engine/engine.h>
#include <engine/psx/graphics.h>
#include <psxcd.h>

void init() {
	ResetGraph(0);
	CdInit();
}

bool isGameRunning() {
	return true;
}
