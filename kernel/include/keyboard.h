#pragma once

#include "types.h"

#define KEY_SHIFT  0b00000001
#define KEY_CTRL   0b00000010
#define KEY_ALT    0b00000100
#define KEY_CAPS   0b00001000
#define KEY_NUM    0b00010000
#define KEY_SCROLL 0b00100000

typedef struct {
    uint8_t scancode;
    uint8_t c;
    uint8_t modifiers;
    boolean pressed;
} Key;

void set_keyboard_handler(uint32_t addr);
uint8_t get_char(uint8_t scancode);
void keyboard_handler();
void init_keyboard();