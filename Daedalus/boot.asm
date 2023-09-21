 [org 0x7c00]   ; mem origin point is 0x7c00
 
 ; set up stack
 mov bp, 0x8000
 mov sp, bp

 ; start printing 
 push 69
 call print_int
 jmp halt

print_int:
    pusha

    mov bp, sp
    add bp, 0x12

    xor ax, ax
    mov al, [bp]

    push 0x0 ; i = 0
    
    divide_loop:
        push ax ; dividend => remainder
        push 10 ; divisor  => quotient
        call flr_div

        pop ax ; quotient
        ; don't pop remainder cause we need it on the stack
        
        mov bl, [bp-20] ; i++
        add bl, 0x01
        mov [bp-20], bl

        cmp al, 0x0a ; al >= 10
        jge divide_loop

    xor ah, ah
    push ax

    print_loop:
        pop ax
        xor ah, ah
        call __print
        
        mov bl, [bp-20] ; i--
        sub bl, 0x01
        mov [bp-20], bl

        cmp bl, 0x0
        jnl print_loop

    popa
    ret

flr_div:
    pusha

    mov bp, sp
    add bp, 0x12 ; offset off all the registers on stack
    
    mov bl, [bp]
    mov al, [bp+2]
    xor ah, ah

    div bl

    mov [bp],   al ; quotient
    mov [bp+2], ah ; remainder
    
    popa
    ret

__print:
    mov ah, 0x0e    ; set mode to TTY
    add al, '0'
    int 0x10
    ret

halt:
    jmp halt

 times 510-($-$$) db 0
 db 0x55, 0xaa