ENTRY(_start)

SECTIONS {
    . = 0x1000;
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
}