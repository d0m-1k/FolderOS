#pragma once

#include "types.h"

#define VGA_ADDRESS 0xB8000
#define VGA_SIZE_X 80
#define VGA_SIZE_Y 25

#define print_hex32e(num) print("0x"); print_hex((num), 8)
#define print_hex16e(num) print("0x"); print_hex((num), 4)
#define print_hex8e(num)  print("0x"); print_hex((num), 2)
#define print_hex32(num) print_hex((num), 8)
#define print_hex16(num) print_hex((num), 4)
#define print_hex8(num)  print_hex((num), 2)
#define clear()          fill(' ', 0x0F)

uint32_t get_cursor_x();
uint32_t get_cursor_y();
void set_cursor(uint32_t x, uint32_t y);
void fill(uint8_t c, uint8_t color);
void print(string str);
void print_hex(uint32_t num, uint8_t size);
void print_dec(uint32_t num);
void putchar_at(uint8_t c, uint8_t color, uint32_t x, uint32_t y);
void print_at(string str, uint8_t color, uint32_t x, uint32_t y);
void scroll();
void putchar(uint8_t c, uint8_t color);
