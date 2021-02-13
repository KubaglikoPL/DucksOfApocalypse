#pragma once

#include <config.h>
#include <engine/img.h>
#ifdef PLATFORM_PSX
#include <psx/stdint.h>
#else
#include <stdint.h>
#include <stdbool.h>
#endif // PLATFORM_PSX

extern int cameraX;
extern int cameraY;
extern uint32_t FPS;

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define CHAR_WIDTH 8
#define SCREEN_SIZE SCREEN_WIDTH * SCREEN_HEIGHT * 3
#ifdef PLATFORM_PSX
#define PSX_ORDERING_TABLE_SIZE 8
#define PSX_MAX_PRIMITIVES 1024
#endif

extern uint32_t FPS;
extern uint32_t Frames;
extern uint32_t screenWidth;
extern uint32_t screenHeight;
#define FRAME_TIME 1.0f/FPS

void init_rendering();
void clear_screen();
void draw_sprite_tx_offset_imageID(int imageID, int width, int height, int tx_offset_x, int tx_offset_y, int x, int y);
void draw_sprite_imageID(int imageID, int x, int y);
#ifdef OFFSCREEN_BUFFER_RENDER
void draw_sprite(uint8_t* data, int width, int height, int x, int y);
void draw_sprite_tx_offset(uint8_t* data, int full_width, int width, int height, int tx_offset_x, int tx_offset_y, int x, int y);
#endif
#ifdef PLATFORM_PSX
void set_image(int imageID);
void set_image(image_t* image);
void draw_sprite_tx_offset(image_t* image, int width, int height, int tx_offset_x, int tx_offset_y, int x, int y);
#endif
void display_screen();

//charCode - Unicode code point
void draw_char(uint16_t charCode, int x, int y, int width, int tx_offset_x);
void draw_string(const char* text, int x, int y);
void draw_string_centered(const char* text, int x, int y);
uint32_t string_width(const char* text);

void set_camera(int x, int y);
void move_camera(int x, int y);
void store_camera();
void restore_camera();
