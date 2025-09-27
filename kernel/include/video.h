#pragma once

#include "types.h"

#define VGA_ADDRESS 0xA0000
#define VGA_SIZE_X 320
#define VGA_SIZE_Y 200

#define clear() fill(0);

void init_video();
void set_pixel(uint32_t x, uint32_t y, pixel_t color);
void fill(pixel_t color);
void draw_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, pixel_t color);
void draw_filled_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, pixel_t color);
void draw_line(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, pixel_t color);
void draw_circle(int32_t x0, int32_t y0, int32_t radius, pixel_t color);
void draw_filled_circle(int32_t x0, int32_t y0, int32_t radius, pixel_t color);
void draw_triangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, pixel_t color);
void draw_filled_triangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, pixel_t color);
void draw_char(uint32_t x, uint32_t y, char c, pixel_t color);