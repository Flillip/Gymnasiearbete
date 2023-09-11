 mov    ah, 0x0e
 mov    al, 64
 mov    bh, 0xff

 loop:
    not bh
    inc al
    
    cmp al, 'Z' + 1 ; if al == 'Z' + 1
    je  halt        ; goto halt

    cmp bh, 0
    je add
    jmp remove

print:
    int 0x10
    jmp loop

add:
    add al, 32
    jmp print

remove:
    sub al, 32
    jmp print

halt:
    jmp halt

 times 510-($-$$) db 0
 db 0x55, 0xaa