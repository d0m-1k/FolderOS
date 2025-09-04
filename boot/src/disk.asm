disk_load:
    pusha
    push dx
    
    mov ah, 0x02
    mov al, dh
    mov ch, 0x00
    mov dh, 0x00
    mov cl, 0x02
    
    int 0x13
    jc .disk_error
    
    pop dx
    cmp al, dh
    jne .sectors_error
    
    popa
    ret
.disk_error:
    mov si, disk_err_msg
    call print
    jmp $
.sectors_error:
    mov si, sectors_err_msg
    call print
    jmp $

disk_err_msg db "Disk read error!", 0
sectors_err_msg db "Incorrect number of sectors read!", 0