#include <CXXoverC/string.h>
#include <CXXoverC/dynamicArray.h>
#include <CXXoverC/intMap.h>
#include <CXXoverC/consoleOutput.h>
#include <engine/engine.h>
#include <engine/graphics.h>
#include <engine/input.h>
#include <engine/gui.h>
#include <engine/map.h>

ConsoleOutput cout;
gui::VirtualKeyboard virtualKeyboard;
gui::TextMenu testTextMenu;

image* test;

void main_loop();

uint32_t duckX = 0;
uint32_t duckY = 0;

int main() {
	/*string s;
	s.add(123);
	s.add(456);
	s.add(789);
	s.add("Hello World!");

	dynamicArray<int> a;
	a.add(1);
	a.add(3);
	a.erase(0);

	IntMap<float> b;
	b.add(124, 0.2f);
	b.add(617, 123.214307f);
	b.add(0, -0.1723f);
	b.erase(617);*/
	
	//cout.add("Hello World!")->add(1)->add((*a.get(0)))->endl()->flush();

	init();
	graphics::init();
	input::init();

#ifdef PLATFORM_DESKTOP
	test = graphics::loadImage("images/sprite_sheet0.png");
	graphics::font = graphics::loadImage("images/font.png");
	map::terrainImage = graphics::loadImage("images/terrain.png");
#else
	test = graphics::loadImage("\\DATA\\SPRT0.TIM;1");
	graphics::font = graphics::loadImage("\\DATA\\FONT.TIM;1");
	map::terrainImage = graphics::loadImage("\\DATA\\TERRAIN.TIM;1");
#endif

#ifdef PLATFORM_DESKTOP
	map::loadMap("maps/test1.map");
#else
	map::loadMap("\\DATA\\TEST1.MAP;1");
#endif

	//dynamicArray<int> a;
	//a.add(4);
	//a.add(5);
	//a.add(9);
	//printf("%i\n", (*a.get(0)));
	//printf("%i\n", (*a.get(1)));
	//printf("%i\n", (*a.get(2)));
	//a.clear();
	//a.add(15);
	//printf("%i\n", (*a.get(0)));

	testTextMenu.entries.add(string("1: Test"));
	testTextMenu.entries.add(string("2: ABC"));
	testTextMenu.entries.add(string("3: AAAAAAA"));

	while (isGameRunning()) {
		main_loop();
	}

	return 0;
}

void main_loop() {
	input::update();
	graphics::clearSprites();
	map::drawMap();
	graphics::drawSprite(test, 16, 16, input::cursorX, input::cursorY);
	graphics::drawString("AA BB CC!", 0, 24);

	//virtualKeyboard.update();
	testTextMenu.update(160, 40);

	graphics::flush_and_display();
}
