#pragma once

#include "types.h"

#define PAGE_SIZE 4096

typedef struct {
    uint32_t base_low;
    uint32_t base_high;
    uint32_t length_low;
    uint32_t length_high;
    uint32_t type;
    uint32_t acpi;
} __attribute__((packed)) bl_memory_map_entry;

void memory_init();
void mark_region_used(uint32_t base, uint32_t size);
void* alloc_page();
void free_page(void* ptr);
uint32_t get_total_memory();