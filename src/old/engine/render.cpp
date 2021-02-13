#include <engine/render.h>
#include <engine/img.h>
#include <malloc.h>
#include <string.h>
#include <engine/text.h>
#ifdef PLATFORM_DESKTOP
#include <GLFW/glfw3.h>
#include <GL/GL.h>
#endif

int cameraX = 0;
int cameraY = 0;
int stored_cameraX = 0;
int stored_cameraY = 0;
uint32_t FPS;
uint32_t Frames = 0;

#ifdef OFFSCREEN_BUFFER_RENDER
uint8_t* buffer;

void init_rendering() {
	buffer = (uint8_t*)malloc(SCREEN_SIZE);
#ifdef PLATFORM_DESKTOP
	FPS = 60;
#endif // PLATFORM_DESKTOP
}

void clear_screen() {
	memset(buffer, 128, SCREEN_SIZE);
}

void draw_sprite(uint8_t* data, int width, int height, int x, int y) {
	draw_sprite_tx_offset(data, width, width, height, 0, 0, x, y);
}

void draw_sprite_imageID(int imageID, int x, int y) {
	image_t* img = &images[imageID];
	draw_sprite(img->data, img->width, img->height, x, y);
}

void draw_sprite_tx_offset(uint8_t* data, int full_width, int width, int height, int tx_offset_x, int tx_offset_y, int x, int y) {
	int offsetX = cameraX + x;
	int offsetY = cameraY + y;

	int txOffsetX = tx_offset_x;
	int txOffsetY = tx_offset_y;

	int txOffset = 0;
	int bufferOffset = 0;

	unsigned char r = 0, g = 0, b = 0;

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			txOffset = (txOffsetX + (txOffsetY * full_width)) * 3;
			
			bufferOffset = (offsetX + (offsetY * SCREEN_WIDTH)) * 3;

			if (((txOffset + 2) < SCREEN_SIZE) && (txOffset > 0)) {
				r = data[txOffset];
				g = data[txOffset + 1];
				b = data[txOffset + 2];
			}

			if (!((r == 109) && (g == 0) && (b == 85))) {
				if (((bufferOffset + 2) < SCREEN_SIZE) && (bufferOffset > 0)) {
					buffer[bufferOffset] = r;
					buffer[bufferOffset + 1] = g;
					buffer[bufferOffset + 2] = b;
				}
			}

			txOffsetX++;
			offsetX++;
		}
		txOffsetY++;
		txOffsetX = tx_offset_x;
		offsetY++;
		offsetX = cameraX + x;
	}
}

void draw_sprite_tx_offset_imageID(int imageID, int width, int height, int tx_offset_x, int tx_offset_y, int x, int y) {
	image_t* img = &images[imageID];
	draw_sprite_tx_offset(img->data, img->width, width, height, tx_offset_x, tx_offset_y, x, y);
}
#endif

#ifdef PLATFORM_DESKTOP
GLuint glTexture = 0;

void display_screen() {
	Frames++;
	if (glTexture == 0) {
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glDisable(GL_DITHER);

		glGenTextures(1, &glTexture);
		glBindTexture(GL_TEXTURE_2D, glTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);
		
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
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, buffer);
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
}
#endif

void draw_char(uint16_t charCode, int x, int y, int width, int tx_offset_x) {
	if (charCode < 128) {
		//Base Font Char
		if(charCode > 31) {
			unsigned char fontCharCode = charCode - 32;
			unsigned char fontCharRow = fontCharCode / 32;
			unsigned char fontCharCol = fontCharCode % 32;

			draw_sprite_tx_offset_imageID(IMAGE_FONT, width, 12, (fontCharCol * 8) + tx_offset_x, fontCharRow * 12, x, y);
		}
	}
	else {
		//Extended Font Char
		//TODO
	}
}

void draw_string(const char* text, int x, int y) {
	int i = 0;
	int offset_x = x;
	uint16_t unicode_point = 0;
	while (text[i]) {
		i += utf8_convert(&unicode_point, text, i);
		//int char_width = get_char_width(unicode_point);
		int char_width = 8;
		int char_tx_offset_x = (8 - char_width) / 2;
		//if ((char_width % 2) == 0) char_tx_offset_x--;
		draw_char(unicode_point, offset_x, y, char_width, char_tx_offset_x);
		offset_x += char_width;
	}
}

uint32_t string_width(const char* text) {
	int i = 0;
	int offset_x = 0;
	uint16_t unicode_point = 0;
	while (text[i]) {
		i += utf8_convert(&unicode_point, text, i);
		offset_x += 8;
	}

	return offset_x;
}

void draw_string_centered(const char* text, int x, int y) {
	uint32_t string_x = x - (string_width(text) / 2);
	draw_string(text, string_x, y);
}

void set_camera(int x, int y) {
	cameraX = x;
	cameraY = y;
}

void move_camera(int x, int y) {
	cameraX = cameraX + x;
	cameraY = cameraY + y;
}

void store_camera() {
	stored_cameraX = cameraX;
	stored_cameraY = cameraY;
}

void restore_camera() {
	cameraX = stored_cameraX;
	cameraY = stored_cameraY;
}
