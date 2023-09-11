 mov    ah, 0x0e
 mov    al, 64
 
 loop:
    inc al
    
    cmp al, 'Z' + 1 ; if al == 'Z' + 1
    je  halt        ; goto halt

    int 0x10
    jmp loop

halt:
    jmp halt

 times 510-($-$$) db 0
 db 0x55, 0xaa