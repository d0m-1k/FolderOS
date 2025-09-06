#include "timer.h"
#include "ports.h"

volatile uint64_t timer_ticks = 0;

void timer_handler() {
    timer_ticks++;
}

void sleep(uint64_t milliseconds) {
    uint64_t target_ticks = timer_ticks + milliseconds;
    while (timer_ticks < target_ticks) {
        __asm__ __volatile__("hlt");
    }
}

void init_timer() {
    uint32_t divisor = PIT_BASE_FREQUENCY / PIT_FREQUENCY;

    outb(0x43, 0x36);
    outb(0x40, divisor & 0xFF);
    outb(0x40, (divisor >> 8) & 0xFF);

    outb(0x21, inb(0x21) & 0xFE);
}