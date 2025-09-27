org 0x7C00
bits 16

global _start

_start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    mov ah, 0x01
    mov ch, 0x3F
    int 0x10

    mov si, WELCOME_MSG
    call print

    call get_memory_map
    
    mov ax, 0x0013
    int 0x10

    mov bx, KERNEL_OFFSET
    mov dh, 21
    call disk_load

    mov si, KERNEL_LOADED_MSG
    call print

    call switch_to_pm

    jmp $

%include "src/print.asm"
%include "src/gdt.asm"
%include "src/disk.asm"
%include "src/memory.asm"
%include "src/modes.asm"

WELCOME_MSG db "Hello world!", 0x0D, 0x0A, 0
KERNEL_LOADED_MSG db "Kernel loaded!", 0x0D, 0x0A, 0
KERNEL_OFFSET equ 0x2000

times 510-($-$$) db 0
dw 0xAA55
