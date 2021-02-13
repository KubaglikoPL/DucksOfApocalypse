#include <config.h>
#include <engine/tar.h>
#include <engine/img.h>
#include <engine/render.h>
#include <engine/text.h>
#include <engine/input.h>
#include <engine/gui.h>
#include <map.h>
#include <game_gui.h>

#ifdef PLATFORM_DESKTOP
#include <GLFW/glfw3.h>
GLFWwindow* window;
#endif
#ifdef PLATFORM_PSX
#include <psxcd.h>
#include <psxetc.h>
#include <psxgte.h>
#include <psxgpu.h>
#endif

void main_loop();

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>

EM_BOOL main_loop_callback(double time, void* userData) {
	main_loop();

	return EM_TRUE;
}

#endif

bool dataLoaded = false;

int main(int argc, char** argv) {
#ifdef PLATFORM_PSX
	initInput();
	cursorX = 320;
	cursorY = 240;
	init_rendering();
	CdInit();
#endif
#ifdef PACKED_GAME_DATA
	dataLoaded = load_tar_file("data.tar");
#endif
#ifdef PLATFORM_DESKTOP
	load_image("font.tga", IMAGE_FONT);
	load_image("gui.tga", IMAGE_GUI);
	load_image("sprite_sheet0.tga", IMAGE_SPRITE_SHEET0);
	load_image("terrain.tga", IMAGE_TERRAIN);
	loadMap("test.map");
#elif defined(PLATFORM_PSX)
	load_image("\\DATA\\FONT.TIM;1" , IMAGE_FONT);
	load_image("\\DATA\\GUI.TIM;1", IMAGE_GUI);
	load_image("\\DATA\\SPRT0.TIM;1", IMAGE_SPRITE_SHEET0);
	load_image("\\DATA\\TERRAIN.TIM;1", IMAGE_TERRAIN);
	loadMap("\\DATA\\TEST.MAP;1");
#endif
	//load_image("base_font.tga", IMAGE_BASE_FONT);
	//load_image("extended_font_polish.tga", IMAGE_EXTENDED_FONT);

#ifdef PLATFORM_DESKTOP
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	
	window = glfwCreateWindow(960, 720, "Window", NULL, NULL);
	initInput(window);
	glfwMakeContextCurrent(window);
#ifndef __EMSCRIPTEN__
	glfwSwapInterval(1);
#endif

	init_rendering();
#endif // PLATFORM_DESKTOP

#ifdef PLATFORM_DESKTOP
#ifndef __EMSCRIPTEN__
	while (!glfwWindowShouldClose(window)) {
		main_loop();
	}
#else
	//emscripten_set_main_loop(main_loop, 60, false);
	emscripten_request_animation_frame_loop(main_loop_callback, 0);
#endif
#else
	while (true) main_loop();
#endif // PLATFORM_DESKTOP
}

void main_loop() {
	updateInput();

	clear_screen();
	//draw_sprite_tx_offset_imageID(IMAGE_BASE_FONT, 8, 12, 8, 12, 0, 12);
#ifdef REVERSED_DRAW_ORDER
	gui_main();
	if (activeGUI != GameGUI::MAIN_MENU) {
		draw_cursor();
		set_image(IMAGE_GUI);
		//draw_string("!!! AABBCCD abcd", 0, 24);
		//draw_string("Hello World!", 0, 13);
		//set_image(IMAGE_FONT);
		renderMap();
		set_image(IMAGE_TERRAIN);
	}
#else
	if (activeGUI != GameGUI::MAIN_MENU) {
		renderMap();
		//draw_string("Hello World!", 0, 13);
		//draw_string("!!! AABBCCD abcd", 0, 24);
		draw_cursor();
	}
	gui_main();
#endif // REVERSED_DRAW_ORDER
#ifdef PLATFORM_PSX
	if (dataLoaded) {
		FntPrint(-1, "HELLO WORLD!");
	}
#endif

	display_screen();
#ifdef PLATFORM_DESKTOP
	glfwSwapBuffers(window);
#endif
}
