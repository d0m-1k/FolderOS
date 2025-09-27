#include "shell.h"
#include "vga.h"
#include "keyboard.h"
#include "timer.h"
#include "types.h"

#define MAX_INPUT_LENGTH 80

static char input_buffer[MAX_INPUT_LENGTH];
static uint32_t input_length = 0;
static uint32_t cursor_position = 0;
static boolean cursor_visible = true;
static uint32_t input_start_x = 0;
static uint32_t input_start_y = 0;

int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

int strncmp(const char* s1, const char* s2, size_t n) {
    while (n && *s1 && (*s1 == *s2)) {
        s1++;
        s2++;
        n--;
    }
    if (n == 0) return 0;
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

void update_cursor() {
    if (cursor_visible) {
        uint32_t current_x = get_cursor_x();
        uint32_t current_y = get_cursor_y();
        putchar_at('_', 0xF0, input_start_x + cursor_position, input_start_y);
        set_cursor(current_x, current_y);
    }
}

void clear_cursor() {
    uint32_t current_x = get_cursor_x();
    uint32_t current_y = get_cursor_y();
    
    if (cursor_position < input_length) {
        putchar_at(input_buffer[cursor_position], 0x0F, 
                  input_start_x + cursor_position, input_start_y);
    } else {
        putchar_at(' ', 0x0F, input_start_x + cursor_position, input_start_y);
    }
    
    set_cursor(current_x, current_y);
}

void draw_input() {
    for (uint32_t i = 0; i < MAX_INPUT_LENGTH; i++) {
        putchar_at(' ', 0x0F, input_start_x + i, input_start_y);
    }
    
    for (uint32_t i = 0; i < input_length; i++) {
        putchar_at(input_buffer[i], 0x0F, input_start_x + i, input_start_y);
    }
    
    update_cursor();
}

void execute_command() {
    input_buffer[input_length] = '\0';
    
    print("\n");
    
    if (strcmp(input_buffer, "help") == 0) {
        print("Available commands: help, clear, echo, sleep\n");
    } else if (strcmp(input_buffer, "clear") == 0) {
        clear();
        input_start_x = 0;
        input_start_y = 0;
    } else if (strncmp(input_buffer, "echo", 4) == 0) {
        print(input_buffer + 5);
        print("\n");
    } else if (input_length > 0) {
        print("Unknown command: ");
        print(input_buffer);
        print("\n");
    }
    
    input_length = 0;
    cursor_position = 0;
    
    input_start_x = get_cursor_x();
    input_start_y = get_cursor_y();
    
    print("> ");
    input_start_x += 2;
    
    draw_input();
}

void key_handler(Key key) {
    if (key.pressed) {
        clear_cursor();
        
        if (key.c == '\n') {
            execute_command();
        } else if (key.c == '\b') {
            if (input_length > 0 && cursor_position > 0) {
                for (uint32_t i = cursor_position; i < input_length; i++) {
                    input_buffer[i - 1] = input_buffer[i];
                }
                input_length--;
                cursor_position--;
                draw_input();
            }
        } else if (key.c != 0 && input_length < MAX_INPUT_LENGTH - 1) {
            if (cursor_position < input_length) {
                for (uint32_t i = input_length; i > cursor_position; i--) {
                    input_buffer[i] = input_buffer[i - 1];
                }
            }
            input_buffer[cursor_position] = key.c;
            input_length++;
            cursor_position++;
            draw_input();
        }
        
        update_cursor();
    }
}

void run_shell() {
    clear();
    print("Folder OS v1.0\n");
    print("Type 'help' for available commands\n");
    
    input_length = 0;
    cursor_position = 0;
    
    input_start_x = get_cursor_x();
    input_start_y = get_cursor_y();
    
    print("> ");
    input_start_x += 2;
    
    draw_input();
    set_keyboard_handler((uint32_t)key_handler);
    
    uint64_t last_blink = timer_ticks;
    
    while (1) {
        sleep(500);
        cursor_visible = !cursor_visible;
        if (cursor_visible)
            update_cursor();
        else
            clear_cursor();
        last_blink = timer_ticks;
        
        __asm__ __volatile__("hlt");
    }
}
