    [org 0x7c00]   ; mem origin point is 0x7c00
    mov [disknum], dl ; save disk number
    
    mov bp, 0x9000 ; set up stack
    mov sp, bp

    call main
    jmp halt


main:
    push 0x0201 ; function and sector
    push 0x0002 ; cylinder and sector num
    push 0x00   ; head nr
    call read_disk
    add sp, 0x06 ; clean stack

    mov ah, 0x0e
    mov al, [0x7e00]
    int 0x10

    ret

read_disk:    
    pusha

    mov bp, sp  
    add bp, 0x12 ; move to before pusha

    mov ax, [bp+0x04]   ; function and sector
    mov cx, [bp+0x02]   ; cylinder and sector number
    mov dx, [bp]        ; head number

    mov dl, [disknum] ; drive number

    mov bx, 0
    mov es, bx
    mov bx, 0x7e00

    int 0x13

    jc __rd_err ; if carry = 1, there was an error

    cmp al, 1   ; if al = 1, we succesfully read one sector
    je __rd_success
    jmp __rd_err

    __rd_err:
        push error_str
        call print_msg_addr
        add sp, 2
        jmp __rd_end

    __rd_success:
        push success_str
        call print_msg_addr
        add sp, 2
        jmp __rd_end

    __rd_end:
        popa
        ret

print_msg_addr:
    pusha
    mov bp, sp
    mov bx, [bp+0x12]
    mov ah, 0x0e

    __print:
        mov al, [bx]
        cmp al, 0
        je __print_end

        int 0x10
        inc bx
        jmp __print

    __print_end:
        mov ah, 0x0e
        mov al, 0x0d
        int 0x10
        mov al, 0x0a
        int 0x10

        popa
        ret

halt:
    jmp halt

disknum: db 0x00
error_str: db "There was an error reading the disk", 0
success_str: db "Successfully read disk", 0


    times 510-($-$$) db 0
    db 0x55, 0xaa

    ; ---- begin disk ---- ;

    times 512 db 'A'