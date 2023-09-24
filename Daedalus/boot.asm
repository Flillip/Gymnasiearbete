    [org 0x7c00]   ; mem origin point is 0x7c00

    KERNEL_LOCATION equ 0x1000

    BOOT_DISK db 0
    mov [BOOT_DISK], dl

    mov bp, 0x8000
    mov sp, bp


    ; set up everything for read disk
    mov dh, 0x00        ; head nr
    mov dl, [BOOT_DISK] ; disk nr
    push 0x0214         ; function & nr of sectors to read
    push 0x0002         ; cylinder nr and sector number
    push dx
    push KERNEL_LOCATION ; where to place code
    call read_disk

    mov ah, 0x0
    mov al, 0x3
    int 0x10

    CODE_SEG equ GDT_code - GDT_start
    DATA_SEG equ GDT_data - GDT_start

    cli
    lgdt [GDT_descriptor]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp CODE_SEG:start_protected_mode

    jmp $

error_str db "err reading disk", 0
success_str db "succ reading disk", 0

; usage:
; push onto stack in the following order:
; -> ax || function and nr of sectors to read
; -> cx || cylinder nr and sector number
; -> dx || head nr & drive number
; -> location to place loaded content
read_disk:    
    pusha

    mov bp, sp  
    add bp, 0x12 ; move to before pusha

    mov ax, [bp+0x06]   ; function and sector
    mov cx, [bp+0x04]   ; cylinder and sector number
    mov dx, [bp+0x02]   ; head number

    mov bx, 0
    mov es, bx
    mov bx, [bp] ; location to put it at

    int 0x13

    jc __rd_err ; if carry = 1, there was an error

    mov cx, [bp+0x06]
    cmp al, cl   ; if al = 1, we succesfully read one sector
    je __rd_success
    jmp __rd_err

    __rd_err:
        push error_str
        call print_msg_addr
        add sp, 2
        jmp $

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


GDT_start:
    GDT_null:
        dd 0x0
        dd 0x0

    GDT_code:
        dw 0xffff
        dw 0x0
        db 0x0
        db 0b10011010
        db 0b11001111
        db 0x0

    GDT_data:
        dw 0xffff
        dw 0x0
        db 0x0
        db 0b10010010
        db 0b11001111
        db 0x0

    GDT_end:

    GDT_descriptor:
        dw GDT_end - GDT_start - 1
        dd GDT_start


[bits 32]
start_protected_mode:
    mov ax, DATA_SEG
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	
	mov ebp, 0x90000		; 32 bit stack base pointer
	mov esp, ebp

    jmp KERNEL_LOCATION
  

    times 510-($-$$) db 0
    db 0x55, 0xaa