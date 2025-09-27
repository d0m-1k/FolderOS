memory_map_offset equ 0x1000
memory_map_entries dd 0

get_memory_map:
    pusha
    xor ax, ax
    mov es, ax
    xor ebx, ebx
    mov di, memory_map_offset + 4
    mov edx, 0x534D4150
    mov eax, 0xE820
    mov ecx, 24
    int 0x15
    jc .error
    inc dword [memory_map_entries]
.next_entry:
    test ebx, ebx
    jz .done

    add di, 24
    mov eax, 0xE820
    mov ecx, 24
    int 0x15
    jc .error

    inc dword [memory_map_entries]
    jnc .next_entry
.error:
    mov si, get_memory_map_error
    call print
    jmp $
.done:
    mov eax, [memory_map_entries]
    mov [memory_map_offset], eax
    popa
    ret

get_memory_map_error db "Error getting memory map", 0