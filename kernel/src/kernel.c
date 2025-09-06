#include "kernel.h"
#include "types.h"
#include "vga.h"
#include "idt.h"
#include "ports.h"
#include "keyboard.h"
#include "shell.h"
#include "timer.h"

void on_error(uint8_t num, uint32_t code);

void _start() {
    clear();
    print("Hello world!\n");

    init_idt();
    init_timer();
    init_keyboard();
    idt_set_handler((uint32_t) on_error);

    sleep(1);

    run_shell();

    while (1) __asm__ __volatile__("hlt");
}

void on_error(uint8_t num, uint32_t code) {
    print("on_error(");
    print_hex(num);
    print(", ");
    print_hex(code);
    print(");\n");
}