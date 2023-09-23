 [org 0x7c00]   ; mem origin point is 0x7c00
 
 ; set up stack
 mov bp, 0x9000
 mov sp, bp

 ; 06     = 6
 ; 0x93c0 = 37824

 xor bx, bx
 jmp start_read

; read enter terminated input string using the stack
; and print it out
start_read:
    mov sp, bp
    push 0x00
    read:
        call get_input
        cmp al, 0x00 ; input = null
        je read

        mov ah, 0
        push ax

        mov bl, [bp-2] ; i++
        add bl, 1
        mov [bp-2], bl

        cmp al, 0x0d
        jne read

        mov bl, [bp-2] ; i--
        sub bl, 1
        mov [bp-2], bl

        jmp start_print_str

start_print_str:
    ; jmp halt
    push bp
    sub bp, 0x02
    mov bl, [bp] ; i--
    ; sub bl, 0x02 ; remove cr/lf
    print_str:
        sub bl, 1

        sub bp, 0x02 ; bp = bp - 2

        mov ah, 0x0e
        mov al, [bp]
        int 0x10

        cmp bl, 0 ; i > 0
        jg print_str ; then start read

        mov ah, 0x0e
        mov al, 13 ; cr
        int 0x10
        mov al, 10 ; lf
        int 0x10

        pop bp

        jmp start_read

; output: al - ascii code, ah - scancode
get_input:
    mov ah, 0
    int 0x16
    ret

halt:
    jmp halt

 times 510-($-$$) db 0
 db 0x55, 0xaa