#include <CXXoverC/string.h>
#include <CXXoverC/dynamicArray.h>
#include <CXXoverC/intMap.h>
#include <CXXoverC/consoleOutput.h>
#include <engine/engine.h>
#include <engine/graphics.h>

ConsoleOutput cout;

int main() {
	string s;
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
	b.erase(617);
	
	cout.add("Hello World!")->add(1)->add((*a.get(0)))->endl()->flush();

	init();
	graphics::init();

	return 0;
}
