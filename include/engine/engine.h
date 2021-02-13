#pragma once

#include <engine/config.h>
#ifdef GLFW_LIB
#include <GLFW/glfw3.h>

extern GLFWwindow* window;
#endif // GLFW_LIB

void init();
