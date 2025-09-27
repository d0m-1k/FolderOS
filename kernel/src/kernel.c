#include "kernel.h"
#include "types.h"
#include "vga.h"
#include "idt.h"
#include "ports.h"
#include "keyboard.h"
#include "shell.h"
#include "timer.h"
#include "serial.h"
#include "memory.h"

void on_error(uint8_t num, uint32_t code);

void _start() {
    clear();
    print("Starting OS!\n");

    init_idt();
    init_timer();
    init_keyboard();
    idt_set_handler((uint32_t) on_error);
    serial_init();
    memory_init();

    print("Memory: ");
    print_dec(get_total_memory()/1024/1024);
    print("MB\n");

    sleep(1000);

    run_shell();

    while (1) __asm__ __volatile__("hlt");
}

void on_error(uint8_t num, uint32_t code) {
    print("on_error(");
    print_hex8e(num);
    print(", ");
    print_hex32e(code);
    print(");\n");
}