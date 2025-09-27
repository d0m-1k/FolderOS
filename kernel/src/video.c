#include "video.h"

static uint8_t font[0xFFFF][8] = {0};

void init_video() {
    fill(0);

    font['A'][0] = 0b00011000;
    font['A'][1] = 0b00100100;
    font['A'][2] = 0b01000010;
    font['A'][3] = 0b01000010;
    font['A'][4] = 0b01111110;
    font['A'][5] = 0b01000010;
    font['A'][6] = 0b01000010;
    font['A'][7] = 0b01000010;

}

void set_pixel(uint32_t x, uint32_t y, pixel_t color) {
    char* video = (char*) VGA_ADDRESS;
    video[y * VGA_SIZE_X + x] = color;
}

void fill(pixel_t color) {
    for (uint32_t y = 0; y < VGA_SIZE_Y; y++) {
        for (uint32_t x = 0; x < VGA_SIZE_X; x++) {
            set_pixel(x, y, color);
        }
    }
}

void draw_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, pixel_t color) {
    for (uint32_t j = 0; j < h; j++) {
        for (uint32_t i = 0; i < w; i++) {
            set_pixel(x + i, y + j, color);
        }
    }
}

void draw_filled_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, pixel_t color) {
    draw_rect(x, y, w, h, color);
}

void draw_line(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, pixel_t color) {
    int dx = x1 - x0;
    int dy = y1 - y0;
    int sx = dx > 0 ? 1 : -1;
    int sy = dy > 0 ? 1 : -1;
    dx = dx > 0 ? dx : -dx;
    dy = dy > 0 ? dy : -dy;

    if (dx > dy) {
        int err = dx / 2;
        for (; x0 != x1; x0 += sx) {
            set_pixel(x0, y0, color);
            err -= dy;
            if (err < 0) {
                y0 += sy;
                err += dx;
            }
        }
    } else {
        int err = dy / 2;
        for (; y0 != y1; y0 += sy) {
            set_pixel(x0, y0, color);
            err -= dx;
            if (err < 0) {
                x0 += sx;
                err += dy;
            }
        }
    }
}

void draw_circle(int32_t x0, int32_t y0, int32_t radius, pixel_t color) {
    int32_t x = radius;
    int32_t y = 0;
    int32_t err = 0;

    while (x >= y) {
        set_pixel(x0 + x, y0 + y, color);
        set_pixel(x0 + y, y0 + x, color);
        set_pixel(x0 - y, y0 + x, color);
        set_pixel(x0 - x, y0 + y, color);
        set_pixel(x0 - x, y0 - y, color);
        set_pixel(x0 - y, y0 - x, color);
        set_pixel(x0 + y, y0 - x, color);
        set_pixel(x0 + x, y0 - y, color);

        if (err <= 0) {
            y += 1;
            err += 2*y + 1;
        }
        if (err > 0) {
            x -= 1;
            err -= 2*x + 1;
        }
    }
}

void draw_filled_circle(int32_t x0, int32_t y0, int32_t radius, pixel_t color) {
    for (int32_t y = -radius; y <= radius; y++) {
        for (int32_t x = -radius; x <= radius; x++) {
            if (x*x + y*y <= radius*radius) {
                set_pixel(x0 + x, y0 + y, color);
            }
        }
    }
}

void draw_triangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, pixel_t color) {
    draw_line(x0, y0, x1, y1, color);
    draw_line(x1, y1, x2, y2, color);
    draw_line(x2, y2, x0, y0, color);
}

void draw_filled_triangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, pixel_t color) {
    if (y0 > y1) { int32_t tx = x0; int32_t ty = y0; x0 = x1; y0 = y1; x1 = tx; y1 = ty; }
    if (y0 > y2) { int32_t tx = x0; int32_t ty = y0; x0 = x2; y0 = y2; x2 = tx; y2 = ty; }
    if (y1 > y2) { int32_t tx = x1; int32_t ty = y1; x1 = x2; y1 = y2; x2 = tx; y2 = ty; }
    int32_t total_height = y2 - y0;
    for (int32_t i = 0; i < total_height; i++) {
        boolean second_half = i > y1 - y0 || y1 == y0;
        int32_t segment_height = second_half ? y2 - y1 : y1 - y0;
        int32_t alpha = i * 1000 / total_height;
        int32_t beta = (i - (second_half ? y1 - y0 : 0)) * 1000 / segment_height;
        int32_t ax = x0 + (x2 - x0) * alpha / 1000;
        int32_t bx = second_half ? x1 + (x2 - x1) * beta / 1000 : x0 + (x1 - x0) * beta / 1000;
        if (ax > bx) { int32_t tx = ax; ax = bx; bx = tx; }
        for (int32_t j = ax; j <= bx; j++) {
            set_pixel(j, y0 + i, color);
        }
    }
}

void draw_char(uint32_t x, uint32_t y, char c, pixel_t color) {
    uint8_t char_index = (uint8_t)c;
    for (int j = 0; j < 8; j++) {
        for (int i = 0; i < 8; i++) {
            boolean pixel = font[char_index][j] & (1 << (7 - i));
            if (pixel) {
                set_pixel(x + i, y + j, color);
            }
        }
    }
}