#include "kernel.h"
#include "types.h"

void clear();
void print(string str);

void main() {
    
    clear();
    print("Hello world!");

    while (1);
}

void clear() {
    uint16_t* vga = (uint16_t*) VGA_ADDRESS;
    for (int i = 0; i < 80*25; i++) {
        vga[i] = 0x0F00;
    }
}

void print(string str) {
    uint16_t* vga = (uint16_t*) VGA_ADDRESS;
    for (int i = 0; str[i] != '\0'; i++) {
        vga[i] = (uint16_t)str[i] | (uint16_t)0x0F00;
    }
}