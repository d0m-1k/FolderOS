#include "shell.h"
#include "video.h"
#include "keyboard.h"
#include "timer.h"
#include "types.h"

void show_loading() {
    // draw_line(0, 0, VGA_SIZE_X-1, VGA_SIZE_Y-1, 15);
    draw_char(15, 15, 'A', 15);
    sleep(500);
}

void run_shell() {
    show_loading();
}
