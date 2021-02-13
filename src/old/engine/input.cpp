#include <engine/input.h>
#include <engine/render.h>
#include <stdio.h>

uint16_t keyState = 0;
uint16_t prevKeyState = 0;
int16_t cursorX;
int16_t cursorY;

#ifdef PLATFORM_DESKTOP
void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if ((action == GLFW_PRESS) || (action == GLFW_REPEAT)) {
		if ((key == GLFW_KEY_UP) || (key == GLFW_KEY_W)) keyState |= KEY_UP;
		if ((key == GLFW_KEY_DOWN) || (key == GLFW_KEY_S)) keyState |= KEY_DOWN;
		if ((key == GLFW_KEY_LEFT) || (key == GLFW_KEY_A)) keyState |= KEY_LEFT;
		if ((key == GLFW_KEY_RIGHT) || (key == GLFW_KEY_D)) keyState |= KEY_RIGHT;
		if (key == GLFW_KEY_ENTER) keyState |= KEY_ACTION;
		if (key == GLFW_KEY_E) keyState |= KEY_MENU;
		if (key == GLFW_KEY_ESCAPE) keyState |= KEY_PAUSE;
		if (key == GLFW_KEY_BACKSPACE) keyState |= KEY_BACKSPACE;
	}
	printf("%i \n", keyState);
}

void glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if ((action == GLFW_PRESS) || (action == GLFW_REPEAT)) {
		if (button == GLFW_MOUSE_BUTTON_LEFT) keyState |= KEY_ACTION;
	}
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	int32_t windowWidth, windowHeight;

	glfwGetWindowSize(window, &windowWidth, &windowHeight);

	cursorX = (uint16_t)(xpos * ((float)SCREEN_WIDTH / windowWidth));
	cursorY = (uint16_t)(ypos * ((float)SCREEN_HEIGHT / windowHeight));
}

void initInput(GLFWwindow* window) {
	glfwSetKeyCallback(window, &glfwKeyCallback);
	glfwSetMouseButtonCallback(window, &glfwMouseButtonCallback);
	glfwSetCursorPosCallback(window, &cursor_position_callback);
}

void updateInput() {
	prevKeyState = keyState;
	keyState = 0;
	glfwPollEvents();
	/*printf("Up: %i \n", keyState & KEY_UP);
	printf("Down: %i \n", keyState & KEY_DOWN);
	printf("Left: %i \n", keyState & KEY_LEFT);
	printf("Right: %i \n", keyState & KEY_RIGHT);
	printf("Action: %i \n", keyState & KEY_ACTION);
	printf("Menu: %i \n", keyState & KEY_MENU);
	printf("Pause: %i \n", keyState & KEY_PAUSE);*/
	//printf("X: %i \n", cursorX);
	//printf("Y: %i \n", cursorY);
}

#elif defined(PLATFORM_PSX)
#include <psxpad.h>
#include <psxapi.h>
#include <string.h>

u_char padbuff[2][34];
u_short button;
PADTYPE padData;

int16_t cursorX_s;
int16_t cursorY_s;

uint32_t abs(int32_t v) {
	if (v < 0) return -v;
	else return v;
}

void initInput() {
	InitPAD((char*)&padbuff[0][0], 34, (char*)&padbuff[1][0], 34);
	StartPAD();
	ChangeClearPAD(1);
}

void updateInput() {
	prevKeyState = keyState;
	keyState = 0;

	//printf("%i \n", prevKeyState);

	memcpy((void*)&padData, &padbuff[0], 34);

	//if (padbuff[0][0] == 0)
	//{
		button = *((u_short*)(padbuff[0] + 2));
		if (!(padData.btn & PAD_UP)) keyState |= KEY_UP;
		if (!(padData.btn & PAD_DOWN)) keyState |= KEY_DOWN;
		if (!(padData.btn & PAD_LEFT)) keyState |= KEY_LEFT;
		if (!(padData.btn & PAD_RIGHT)) keyState |= KEY_RIGHT;
		if (!(padData.btn & PAD_CROSS)) keyState |= KEY_ACTION;
		if (!(padData.btn & PAD_SELECT)) keyState |= KEY_MENU;
		if (!(padData.btn & PAD_START)) keyState |= KEY_PAUSE;
		if (!(padData.btn & PAD_CIRCLE)) keyState |= KEY_BACKSPACE;
	//}

	//printf("X: %i \n", padData.ls_x);
	//printf("Y: %i \n", padData.ls_y);

	int32_t analogX = padData.ls_x - 128;
	int32_t analogY = padData.ls_y - 128;
	//cursorX = padData.ls_x;
	//cursorY = padData.ls_y;
	//float analogX_f, analogY_f;

	cursorX = cursorX_s;
	cursorY = cursorY_s;

	if (abs(analogX) > 30) {
		//printf("X: %i \n", analogY);
		cursorX = (analogX / 50) + cursorX;
		if (cursorX < 0) cursorX = 0;
		cursorX_s = cursorX;
	}
	if (abs(analogY) > 30) {
		//printf("Y: %i \n", analogY);
		cursorY = (analogY / 50) + cursorY;
		if (cursorY < 0) cursorY = 0;
		cursorY_s = cursorY;
	}

	//printf("CX: %i \n", cursorX_s);
	//printf("CY: %i \n", cursorY_s);

	/*printf("Up: %i \n", keyState & KEY_UP);
	printf("Down: %i \n", keyState & KEY_DOWN);
	printf("Left: %i \n", keyState & KEY_LEFT);
	printf("Right: %i \n", keyState & KEY_RIGHT);
	printf("Action: %i \n", keyState & KEY_ACTION);
	printf("Menu: %i \n", keyState & KEY_MENU);
	printf("Pause: %i \n", keyState & KEY_PAUSE);*/
}

#endif // PLATFORM_DESKTOP

