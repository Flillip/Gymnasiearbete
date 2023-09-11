 [org 0x7c00]   ; mem origin point is 0x7c00
 mov    ah, 0x0e; set mode to TTY
 mov    bx, str ; moves the mem address of str into bx

 loop:
    mov al, [bx]; get next char in string
    
    cmp al, 0   ; if al == 0
    je  halt    ; goto halt
    
    int 0x10    ; interrupt 0x10, print
    inc bx      ; incremeant bx pointer
    jmp loop    ; loop

halt:
    jmp halt

str:
    db "Goodbye, world", 0

 times 510-($-$$) db 0
 db 0x55, 0xaa