#include <engine/graphics.h>
#include <engine/memory.h>
#include <engine/file.h>
#include <engine/engine.h>
#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_HDR
#define STBI_NO_STDIO
#define STBI_NO_LINEAR
#define STBI_WINDOWS_UTF8
#include <stb_image.h>
#include <GLFW/glfw3.h>
#include <gl/GL.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#ifdef RGB8_SCREEN_BUFFER
#define SCREEN_SIZE SCREEN_WIDTH * SCREEN_HEIGHT * 3
#endif

uint8_t* screenBuffer;

namespace graphics {
	uint32_t Frames = 0;
	uint32_t FPS = 60; 
	uint32_t screenWidth = SCREEN_WIDTH;
	uint32_t screenHeight = SCREEN_HEIGHT;
}

void glfwWindowSizeCallback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}

void graphics::init() {
#ifdef RGB8_SCREEN_BUFFER
	screenBuffer = (uint8_t*)malloc(SCREEN_WIDTH * SCREEN_HEIGHT * 3);
#endif
	glfwSetWindowSizeCallback(window, glfwWindowSizeCallback);
}

image* graphics::loadImage(const char* filepath) {
	uint32_t fileSize = 0;
	uint8_t* fileData = loadFile(filepath, fileSize);
	if (!fileData) return nullptr;

	int w = 0, h = 0, comp = 0;

	uint8_t* imageData = stbi_load_from_memory(fileData, fileSize, &w, &h, &comp, 3);
	free(fileData);
	if ((w == 0) || (h == 0) || (comp == 0) || (imageData == NULL)) {
		return nullptr;
	}

	image* img = new image();

	img->data = imageData;
	img->width = w;
	img->height = h;
	return img;
}

GLuint glTexture;

void drawSpriteInstance(graphics::SpriteInstance* instance) {
	if (instance) {
		int offsetX = instance->x;
		int offsetY = instance->y;

		int txOffsetX = instance->u_offset;
		int txOffsetY = instance->v_offset;

		int txOffset = 0;
		int bufferOffset = 0;

		unsigned char r = 0, g = 0, b = 0;

		uint8_t* data = instance->img->data;

		for (int y = 0; y < instance->height; y++) {
			for (int x = 0; x < instance->width; x++) {
				txOffset = (txOffsetX + (txOffsetY * instance->img->width)) * 3;

				bufferOffset = (offsetX + (offsetY * SCREEN_WIDTH)) * 3;

				if (((txOffset + 2) < SCREEN_SIZE) && (txOffset >= 0)) {
					r = data[txOffset];
					g = data[txOffset + 1];
					b = data[txOffset + 2];
				}

				if (!((r == 109) && (g == 0) && (b == 85))) {
					if (((bufferOffset + 2) < SCREEN_SIZE) && (bufferOffset >= 0)) {
						screenBuffer[bufferOffset] = r;
						screenBuffer[bufferOffset + 1] = g;
						screenBuffer[bufferOffset + 2] = b;
					}
				}

				txOffsetX++;
				offsetX++;
			}
			txOffsetY++;
			txOffsetX = instance->u_offset;;
			offsetY++;
			offsetX = instance->x;
		}
	}
}

void flush() {
#ifdef RGB8_SCREEN_BUFFER
	memset(screenBuffer, 0, SCREEN_WIDTH * SCREEN_HEIGHT * 3);
#endif
	for (uint32_t i = 0; i < graphics::spriteInstances.getSize(); i++) {
		drawSpriteInstance(graphics::spriteInstances.get(i));
	}
}

void graphics::flush_and_display() {
	graphics::Frames++;

	//printf("%i \n", graphics::Frames);

	flush();
	if (glTexture == 0) {
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glDisable(GL_DITHER);

		glGenTextures(1, &glTexture);
		glBindTexture(GL_TEXTURE_2D, glTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, screenBuffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
#ifdef __EMSCRIPTEN__
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
#endif
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
	}
	//glViewport(0, 0, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, screenBuffer);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(-1.0f, -1.0f);

	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(-1.0f, 1.0f);

	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(1.0f, 1.0f);

	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(1.0f, -1.0f);
	glEnd();
	glFlush();

	glfwSwapBuffers(window);
}
