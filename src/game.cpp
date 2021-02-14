#include <CXXoverC/string.h>
#include <CXXoverC/dynamicArray.h>
#include <CXXoverC/intMap.h>
#include <CXXoverC/consoleOutput.h>
#include <engine/engine.h>
#include <engine/graphics.h>

ConsoleOutput cout;

image* test;

void main_loop();

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

#ifdef PLATFORM_DESKTOP
	test = graphics::loadImage("images/font.png");
#else
	test = graphics::loadImage("\\DATA\\FONT.TIM;1");
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

	while (isGameRunning()) {
		main_loop();
	}

	return 0;
}

void main_loop() {
	graphics::clearSprites();
	graphics::drawSprite(test, 0, 0);

	graphics::flush_and_display();
}
