#include "vga.h"

static uint32_t pos_x = 0;
static uint32_t pos_y = 0;

void fill(uint8_t c, uint8_t color) {
    uint16_t* vga = (uint16_t*) VGA_ADDRESS;
    for (int i = 0; i < VGA_SIZE_X*VGA_SIZE_Y; i++) {
        vga[i] = (color << 8) | c;
    }
    pos_x = 0;
    pos_y = 0;
}

void print(string str) {
    uint16_t* vga = (uint16_t*) VGA_ADDRESS;
    for (int i = 0; str[i] != '\0'; i++) {
        putchar(str[i], 0xFF);
    }
}

void scroll() {
    if (pos_y < VGA_SIZE_Y) {
        pos_y++;
        return;
    }
    for (int i = 0; i < VGA_SIZE_Y; i++) {
        uint16_t* line1 = (uint16_t*) VGA_ADDRESS+(i*VGA_SIZE_X);
        uint16_t* line2 = (uint16_t*) VGA_ADDRESS+(i*VGA_SIZE_X+1);
        for (int x = 0; i < VGA_SIZE_X; i++) {
            line1[x] = line2[x];
        }
    }
}

void putchar(uint8_t c, uint8_t color) {
    uint16_t* vga = (uint16_t*) VGA_ADDRESS;

    if (pos_x >= VGA_SIZE_X) {
        pos_x = 0;
        scroll();
    }

    switch (c) {
    case '\n': scroll(); break;
    case '\r': pos_x = 0; break;
    
    default:
        vga[pos_y * VGA_SIZE_X + pos_x] = (uint16_t)c | (uint16_t)0x0F00;
        pos_x++;
    }   
}