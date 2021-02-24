#include <engine/engine.h>
#include <engine/input.h>
#include <engine/graphics.h>

namespace input {
	uint16_t keyState = 0;
	uint16_t cursorX = 0;
	uint16_t cursorY = 0;
}

void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if ((action == GLFW_PRESS) || (action == GLFW_REPEAT)) {
		if ((key == GLFW_KEY_UP) || (key == GLFW_KEY_W)) input::keyState |= KEY_UP;
		if ((key == GLFW_KEY_DOWN) || (key == GLFW_KEY_S)) input::keyState |= KEY_DOWN;
		if ((key == GLFW_KEY_LEFT) || (key == GLFW_KEY_A)) input::keyState |= KEY_LEFT;
		if ((key == GLFW_KEY_RIGHT) || (key == GLFW_KEY_D)) input::keyState |= KEY_RIGHT;
		if (key == GLFW_KEY_ENTER) input::keyState |= KEY_MENU_ACTION;
		if (key == GLFW_KEY_E) input::keyState |= KEY_SELECT;
		if (key == GLFW_KEY_ESCAPE) input::keyState |= KEY_START;
	}
	//printf("%i \n", input::keyState);
}

void glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if ((action == GLFW_PRESS) || (action == GLFW_REPEAT)) {
		if (button == GLFW_MOUSE_BUTTON_LEFT) input::keyState |= KEY_ACTION;
	}
}

void glfwCursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
	int32_t windowWidth, windowHeight;

	glfwGetWindowSize(window, &windowWidth, &windowHeight);

	input::cursorX = (uint16_t)(xpos * ((float)graphics::screenWidth / windowWidth));
	input::cursorY = (uint16_t)(ypos * ((float)graphics::screenHeight / windowHeight));
}

void input::init() {
	glfwSetKeyCallback(window, glfwKeyCallback);
	glfwSetMouseButtonCallback(window, glfwMouseButtonCallback);
	glfwSetCursorPosCallback(window, glfwCursorPositionCallback);
}

void input::update() {
	keyState = 0;
	glfwPollEvents();
}
