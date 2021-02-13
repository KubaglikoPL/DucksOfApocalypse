#pragma once

#include <config.h>
#ifdef PLATFORM_PSX
#include <psx/stdint.h>
#else
#include <stdint.h>
#include <stdbool.h>
#endif // PLATFORM_PSX

#define KEY_UP 1
#define KEY_DOWN 2
#define KEY_LEFT 4
#define KEY_RIGHT 8
#define KEY_ACTION 16 //Left Mouse Button(PC) / Cross(PS1)
#define KEY_MENU 32 //E(PC) / Select(PS1)
#define KEY_PAUSE 64 //Escape(PC) / Start(PS1)
#define KEY_BACKSPACE 128 //Backspace(PC) / Circle(PS1)

extern uint16_t keyState;
extern uint16_t prevKeyState;
extern int16_t cursorX;
extern int16_t cursorY;

void updateInput();

#ifdef PLATFORM_DESKTOP
#include <GLFW/glfw3.h>

	void initInput(GLFWwindow *window);
#elif defined(PLATFORM_PSX)
	void initInput();
#endif // PLATFORM_DESKTOP
