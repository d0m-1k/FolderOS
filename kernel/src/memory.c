#include "memory.h"

static uint8_t* memory_bitmap;
static uint32_t total_pages;
static uint32_t bitmap_size;
static uint32_t memory_start;
static uint32_t total_memory = 0;

void memory_init() {
    uint32_t bl_memory_map_size = *(uint32_t*) 0x1000;
    bl_memory_map_entry* bl_memory_map = (bl_memory_map_entry*) 0x1004;
    
    uint32_t max_address = 0;
    for (uint32_t i = 0; i < bl_memory_map_size; i++) {
        if (bl_memory_map[i].type == 1) {
            if (bl_memory_map[i].base_high == 0) {
                uint32_t base = bl_memory_map[i].base_low;
                uint32_t length = bl_memory_map[i].length_low;
                uint32_t end = base + length;

                total_memory += length;
                
                if (end > base && end > max_address) {
                    max_address = end;
                }
            }
        }
    }
    
    memory_start = 0x100000;
    
    if (max_address <= memory_start) {
        return;
    }
    
    total_pages = (max_address - memory_start) / PAGE_SIZE;
    bitmap_size = (total_pages + 7) / 8;
    
    memory_bitmap = (uint8_t*)0x20000;
    
    for (uint32_t i = 0; i < bitmap_size; i++) {
        memory_bitmap[i] = 0xFF;
    }
    
    for (uint32_t i = 0; i < bl_memory_map_size; i++) {
        if (bl_memory_map[i].type == 1 && bl_memory_map[i].base_high == 0) {
            uint32_t base = bl_memory_map[i].base_low;
            uint32_t length = bl_memory_map[i].length_low;
            
            if (base + length <= memory_start) continue;
            if (base >= max_address) continue;
            
            uint32_t start_addr = (base < memory_start) ? memory_start : base;
            uint32_t end_addr = (base + length > max_address) ? max_address : base + length;
            
            uint32_t start_page = (start_addr - memory_start) / PAGE_SIZE;
            uint32_t end_page = (end_addr - memory_start) / PAGE_SIZE;
            
            for (uint32_t page = start_page; page < end_page; page++) {
                if (page >= total_pages) break;
                
                uint32_t byte_index = page / 8;
                uint32_t bit_index = page % 8;
                memory_bitmap[byte_index] &= ~(1 << bit_index);
            }
        }
    }
    
    mark_region_used(0x0, 0x100000);
    mark_region_used(0x20000, bitmap_size);
}

uint32_t get_total_memory() {
    return total_memory;
}

void mark_region_used(uint32_t base, uint32_t size) {
    if (base < memory_start) {
        if (base + size <= memory_start) return;
        size = size - (memory_start - base);
        base = memory_start;
    }
    
    uint32_t start_page = (base - memory_start) / PAGE_SIZE;
    uint32_t num_pages = (size + PAGE_SIZE - 1) / PAGE_SIZE;
    
    for (uint32_t i = start_page; i < start_page + num_pages; i++) {
        if (i >= total_pages) break;
        
        uint32_t byte_index = i / 8;
        uint32_t bit_index = i % 8;
        memory_bitmap[byte_index] |= (1 << bit_index);
    }
}

void* alloc_page() {
    for (uint32_t i = 0; i < total_pages; i++) {
        uint32_t byte_index = i / 8;
        uint32_t bit_index = i % 8;
        
        if (!(memory_bitmap[byte_index] & (1 << bit_index))) {
            memory_bitmap[byte_index] |= (1 << bit_index);
            return (void*)(memory_start + i * PAGE_SIZE);
        }
    }
    return null;
}

void free_page(void* ptr) {
    if (!ptr) return;
    
    uint32_t addr = (uint32_t)ptr;
    if (addr < memory_start) return;
    
    uint32_t page_index = (addr - memory_start) / PAGE_SIZE;
    if (page_index >= total_pages) return;
    
    uint32_t byte_index = page_index / 8;
    uint32_t bit_index = page_index % 8;
    
    memory_bitmap[byte_index] &= ~(1 << bit_index);
}

uint32_t get_free_pages_count() {
    uint32_t free_count = 0;
    for (uint32_t i = 0; i < total_pages; i++) {
        uint32_t byte_index = i / 8;
        uint32_t bit_index = i % 8;
        if (!(memory_bitmap[byte_index] & (1 << bit_index))) {
            free_count++;
        }
    }
    return free_count;
}