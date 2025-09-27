ENTRY(_start)

SECTIONS {
    . = 0x2000;
    .text : {
        build/kernel.o(.text)
        *(.text)
    }
    .data : {
        *(.data)
    }
    .bss : {
        *(.bss)
    }
    . = ALIGN(4);
    _enda = .;
}