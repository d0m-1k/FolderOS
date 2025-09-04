#pragma once

#include "types.h"

#define VGA_ADDRESS 0xB8000
#define VGA_SIZE_X 80
#define VGA_SIZE_Y 25

#define clear() fill(' ', 0x0F);

void fill(uint8_t c, uint8_t color);
void print(string str);
void set_cursor_position(size_t x, size_t y);
void scroll();
void putchar(uint8_t c, uint8_t color);