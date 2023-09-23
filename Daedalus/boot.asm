    [org 0x7c00]   ; mem origin point is 0x7c00

    mov [BOOT_DISK], dl

    CODE_SEG equ GDT_code - GDT_start
    DATA_SEG equ GDT_data - GDT_start

    cli
    lgdt [GDT_descriptor]
    ; change last bit of cr0 to 1
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; far jump (jmp to other segment)
    jmp CODE_SEG:start_protected_mode


GDT_start:
    GDT_null:
        dd 0x0 ; four times 00000000
        dd 0x0 ; four times 00000000
    
    GDT_code:
        dw 0xffff ; first 16 bits of limit
        dw 0x0 ; 16 bits +
        db 0x0 ; 8 bits = 24
        db 0b10011010 ; present, privelage, type, Type flags
        db 0b11001111 ; Other + limit (last four bits)
        db 0x0 ; Last 8 bits of base

    GDT_data:
        dw 0xffff ; first 16 bits of limit
        dw 0x0 ; 16 bits +
        db 0x0 ; 8 bits = 24
        db 0b10010010 ; present, privelage, type, Type flags
        db 0b11001111 ; Other + limit (last four bits)
        db 0x0 ; Last 8 bits of base
    
GDT_end:

GDT_descriptor:
    dw GDT_end - GDT_start - 1 ; size
    dd GDT_start               ; start

[bits 32]
start_protected_mode:
    mov al, 'A'
    mov ah, 0x0f
    mov [0xb8000], ax

    jmp halt

BOOT_DISK: db 0

halt:
    jmp halt

    times 510-($-$$) db 0
    db 0x55, 0xaa