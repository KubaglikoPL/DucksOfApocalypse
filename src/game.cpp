#include <CXXoverC/string.h>
#include <CXXoverC/dynamicArray.h>

int main() {
	string s;
	s.add(123);
	s.add(456);
	s.add(789);

	dynamicArray<int> a;
	a.add(1);
	a.add(3);
	a.erase(0);

	return 0;
}
