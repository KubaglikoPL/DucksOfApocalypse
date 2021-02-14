#include <engine/engine.h>

#ifdef GLFW_LIB
GLFWwindow* window;
#endif //GLFW_LIB

void init() {
#ifdef GLFW_LIB
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	window = glfwCreateWindow(960, 720, "Window", NULL, NULL);
	//initInput(window);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
#endif //GLFW_LIB
}

bool isGameRunning() {
	return !glfwWindowShouldClose(window);
}
