#include "idt.h"
#include "vga.h"
#include "keyboard.h"
#include "timer.h"

struct idt_entry idt[IDT_ENTRIES];
struct idt_ptr idtp;

uint32_t handler_address = 0;

void idt_set_handler(uint32_t addr) {
    handler_address = addr;
}

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low = (base & 0xFFFF);
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].selector = sel;
    idt[num].zero = 0;
    idt[num].flags = flags;
}

void idt_load(uint32_t idt_ptr) {
    __asm__ __volatile__ (
        "lidtl (%0)"
        :
        : "r" (idt_ptr)
    );
}


#define isr(n) void __attribute__((interrupt)) i##n(void*) { idt_handler(n, 0); }
#define isr_err(n) void __attribute__((interrupt)) i##n(void*, uint32_t err_code) { idt_handler(n, err_code); }

isr(0)
isr(1)
isr(2)
isr(3)
isr(4)
isr(5)
isr(6)
isr(7)
isr_err(8)
isr(9)
isr_err(10)
isr_err(11)
isr_err(12)
isr_err(13)
isr_err(14)
isr(15)
isr(16)
isr_err(17)
isr(18)
isr(19)
isr(32)
isr(33)

#undef isr
#undef isr_err

void __attribute__((interrupt)) default_handler(void*) {
    print("Unhandled exception\n");
    while(1);
}

void init_pic() {
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    
    outb(0x21, 0xFD);
    outb(0xA1, 0xFF);
}

void idt_handler(uint8_t num, uint32_t err_code) {
    if (num == 33) {
        keyboard_handler();
        outb(0x20, 0x20);
    } else if (num == 32) {
        timer_handler();
        outb(0x20, 0x20);
    } else if (handler_address != 0) {
        void (*handler)(uint8_t, uint32_t) = (void (*)(uint8_t, uint32_t)) handler_address;
        handler(num, err_code);
    } else {
        print("Interrupt: ");
        print_hex(num);
        print("\n");
    }
}

void init_idt() {
    init_pic();

    idtp.limit = sizeof(struct idt_entry) * IDT_ENTRIES - 1;
    idtp.base = (uint32_t)&idt;

    for (int i = 0; i < IDT_ENTRIES; i++) {
        idt_set_gate(i, (uint32_t)default_handler, 0, 0);
    }

    #define isr(n) idt_set_gate(n, (uint32_t)i##n, 0x08, 0x8E);
    isr(0)
    isr(1)
    isr(2)
    isr(3)
    isr(4)
    isr(5)
    isr(6)
    isr(7)
    isr(8)
    isr(9)
    isr(10)
    isr(11)
    isr(12)
    isr(13)
    isr(14)
    isr(15)
    isr(16)
    isr(17)
    isr(18)
    isr(19)
    isr(32)
    isr(33)
    #undef isr
    
    idt_load((uint32_t)&idtp);
    
    __asm__ __volatile__("sti");
}