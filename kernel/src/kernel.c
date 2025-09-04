#include "kernel.h"
#include "types.h"
#include "vga.h"

void _start() {
    clear();
    print("Hello world!");

    while (1);
}
