#pragma once

#include "types.h"
#include "ports.h"

#define IDT_ENTRIES 256

struct idt_entry {
	uint16_t base_low;
	uint16_t selector;
	uint8_t zero;
	uint8_t flags;
	uint16_t base_high;
} __attribute__((packed));

struct idt_ptr {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));

void init_idt();
void idt_set_gate(uint8_t, uint32_t, uint16_t, uint8_t);
void idt_load(uint32_t);
void idt_handler(uint8_t, uint32_t);
void idt_set_handler(uint32_t);
void init_pic();

extern struct idt_entry idt[IDT_ENTRIES];
extern struct idt_ptr idtp;