#include <engine/graphics.h>
#include <engine/memory.h>
#include <engine/file.h>
#include <engine/engine.h>
#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_HDR
#define STBI_ONLY_PNG
#define STBI_NO_STDIO
#define STBI_NO_LINEAR
#define STBI_WINDOWS_UTF8
#include <stb_image.h>
#include <GLFW/glfw3.h>
#include <gl/GL.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

uint8_t* screenBuffer;

namespace graphics {
	uint32_t Frames = 0;
	uint32_t FPS = 60;
}

void graphics::init() {
#ifdef RGB8_SCREEN_BUFFER
	screenBuffer = (uint8_t*)malloc(SCREEN_WIDTH * SCREEN_HEIGHT * 3);
#endif
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

void flush() {
#ifdef RGB8_SCREEN_BUFFER
	memset(screenBuffer, 0, SCREEN_WIDTH * SCREEN_HEIGHT * 3);
#endif
}

void graphics::flush_and_display() {
	glfwPollEvents(); //TEMP, FIXME!!!!

	graphics::Frames++;
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
