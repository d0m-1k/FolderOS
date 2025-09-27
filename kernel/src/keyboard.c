#include "keyboard.h"
#include "ports.h"

const char scancode_to_char[128] = {
    0,   0,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 
    '-', '=', '\b', 0,   'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
    'o', 'p', '[', ']', '\n', 0,   'a', 's', 'd', 'f', 'g', 'h',
    'j', 'k', 'l', ';', '\'', '`', 0,   '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', 0,   '*', 0,   ' ', 0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0
};

uint32_t keyboard_handler_address = 0;

void set_keyboard_handler(uint32_t addr) {
    keyboard_handler_address = addr;
}

uint8_t get_char(uint8_t scancode) {
    if (scancode >= 128) return 0;
    return scancode_to_char[scancode];
}

void keyboard_handler() {
    uint8_t scancode = inb(0x60);
    Key key;

    
    if (scancode & 0x80) {
        scancode &= 0x7F;
        key.scancode = scancode;
        key.c = get_char(scancode);
        key.pressed = false;
    } else {
        key.scancode = scancode;
        key.c = get_char(scancode);
        key.pressed = true;
    }
    
    if (keyboard_handler_address != 0) {
        void (*handler)(Key) = (void (*)(Key)) keyboard_handler_address;
        handler(key);
    }
}

void init_keyboard() {
    outb(0x21, inb(0x21) & 0xFD);
}