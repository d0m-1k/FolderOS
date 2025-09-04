org 0x7C00
bits 16

global _start

_start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax

    mov si, WELCOME_TEXT
    call print

    jmp $

%include "src/print"

WELCOME_TEXT db "Hello world!", 0

times 510-($-$$) db 0
dw 0xAA55