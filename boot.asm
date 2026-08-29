bits 16
org 0x7C00

global _start

_start:
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    mov ax, 0x1000
    mov es, ax
    xor bx, bx

    mov ah, 0x02
    mov al, 15
    mov ch, 0
    mov cl, 2
    mov dh, 0
    int 0x13
    jc disk_error

    mov ax, 0x0013
    int 0x10

    cli
    lgdt [gdt_desc]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp 0x08:init_pm

disk_error:
    hlt
    jmp disk_error

bits 32
init_pm:
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov esp, 0x90000
    jmp 0x10000

gdt_start:
    dd 0, 0
gdt_code:
    dw 0xFFFF, 0
    db 0, 0x9A, 0xCF, 0
gdt_data:
    dw 0xFFFF, 0
    db 0, 0x92, 0xCF, 0
gdt_end:

gdt_desc:
    dw gdt_end - gdt_start - 1
    dd gdt_start

times 510-($-$$) db 0
dw 0xAA55
