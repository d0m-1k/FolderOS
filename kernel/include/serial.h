#pragma once

#include "types.h"

#define COM1_PORT 0x3F8

void serial_init(void);
void serial_putc(char c);
void serial_puts(string str);
char serial_getc(void);
int serial_received(void);