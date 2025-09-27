#include "vga.h"
#include "serial.h"

static uint32_t pos_x = 0;
static uint32_t pos_y = 0;

uint32_t get_cursor_x() {
    return pos_x;
}

uint32_t get_cursor_y() {
    return pos_y;
}

void set_cursor(uint32_t x, uint32_t y) {
    if (x >= VGA_SIZE_X) {
        x = 0;
        y++;
    }
    if (y >= VGA_SIZE_Y) {
        scroll();
        y = VGA_SIZE_Y - 1;
    }
    pos_x = x;
    pos_y = y;
}

void fill(uint8_t c, uint8_t color) {
    uint16_t* vga = (uint16_t*) VGA_ADDRESS;
    for (int i = 0; i < VGA_SIZE_X*VGA_SIZE_Y; i++) {
        vga[i] = (color << 8) | c;
    }
    pos_x = 0;
    pos_y = 0;
}

void print(string str) {
    for (int i = 0; str[i] != '\0'; i++) {
        putchar(str[i], 0x0F);
    }
}

void print_hex(uint32_t num, uint8_t size) {
    string chars = "0123456789ABCDEF";
    char buf[size + 1];
    int pos = 0;
    for (int i = size - 1; i >= 0; i--) {
        buf[pos + i] = chars[num & 0x0F];
        num >>= 4;
    }
    pos += size;
    buf[pos] = '\0';
    print(buf);
}

void print_dec(uint32_t num) {
    uint32_t temp = num;
    uint8_t digits = 0;
    
    do {
        digits++;
        temp /= 10;
    } while (temp > 0);
    
    char buf[digits + 1];
    
    buf[digits] = '\0';
    for (int i = digits - 1; i >= 0; i--) {
        buf[i] = '0' + (num % 10);
        num /= 10;
    }
    
    print(buf);
}
void putchar_at(uint8_t c, uint8_t color, uint32_t x, uint32_t y) {
    uint16_t* vga = (uint16_t*) VGA_ADDRESS;
    if (x < VGA_SIZE_X && y < VGA_SIZE_Y) {
        vga[y * VGA_SIZE_X + x] = (uint16_t)c | (uint16_t)(color << 8);
    }
}

void print_at(string str, uint8_t color, uint32_t x, uint32_t y) {
    for (int i = 0; str[i] != '\0'; i++) {
        putchar_at(str[i], color, x + i, y);
    }
}

void scroll() {
    uint16_t* vga = (uint16_t*) VGA_ADDRESS;
    for (int y = 0; y < VGA_SIZE_Y-1; y++) {
        for (int x = 0; x < VGA_SIZE_X; x++) {
            vga[y * VGA_SIZE_X + x] = vga[(y+1) * VGA_SIZE_X + x];
        }
    }
    for (int x = 0; x < VGA_SIZE_X; x++) {
        vga[(VGA_SIZE_Y-1) * VGA_SIZE_X + x] = (uint16_t)' ' | (uint16_t)(0x0F << 8);
    }
    pos_x = 0;
    pos_y = VGA_SIZE_Y-1;
}

void putchar(uint8_t c, uint8_t color) {
    uint16_t* vga = (uint16_t*) VGA_ADDRESS;

    if (c == '\n') {
        pos_x = 0;
        pos_y++;
        serial_putc(c);
    } else if (c == '\r') {
        pos_x = 0;
        serial_putc(c);
    } else if (c == '\b') {
        if (pos_x > 0) {
            pos_x--;
            vga[pos_y * VGA_SIZE_X + pos_x] = (uint16_t)' ' | (uint16_t)(0x0F << 8);
            serial_puts("\b \b");
        }
    } else {
        vga[pos_y * VGA_SIZE_X + pos_x] = (uint16_t)c | (uint16_t)(color << 8);
        serial_putc(c);
        pos_x++;
    }

    if (pos_x >= VGA_SIZE_X) {
        pos_x = 0;
        pos_y++;
        serial_putc('\n');
    }

    if (pos_y >= VGA_SIZE_Y) {
        scroll();
    }
}