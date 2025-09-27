#include "kernel.h"
#include "types.h"
#include "idt.h"
#include "ports.h"
#include "keyboard.h"
#include "shell.h"
#include "timer.h"
#include "serial.h"
#include "memory.h"
#include "video.h"

void on_error(uint8_t num, uint32_t code);

void _start() {
    init_idt();
    init_timer();
    init_keyboard();
    idt_set_handler((uint32_t) on_error);
    serial_init();
    memory_init();
    init_video();

    run_shell();

    while (1) __asm__ __volatile__("hlt");
}

void on_error(uint8_t num, uint32_t code) {
    serial_puts("Exception\r\n");
}