bits 16
section .text
global _start
extern kernel_main
extern keyboard_handler

_start:
    mov ax, 0x0013
    int 0x10

    cli
    lgdt [gdt_desc]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp 0x08:init_pm

bits 32
init_pm:
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov esp, stack_space

    call init_idt
    call init_pic
    call init_keyboard

    sti
    call kernel_main
    hlt

init_idt:
    mov edi, idt
    mov ecx, 256
    mov eax, 0
    rep stosw
    stosw
    stosw
    stosw

    mov eax, keyboard_handler
    mov bx, 0x08
    mov cx, 0x8E00

    mov edi, idt + 33 * 8
    mov [edi], ax
    mov [edi + 2], bx
    mov [edi + 4], cx
    shr eax, 16
    mov [edi + 6], ax

    lidt [idt_desc]
    ret

init_pic:
    mov al, 0x11
    out 0x20, al
    out 0xA0, al

    mov al, 0x20
    out 0x21, al
    mov al, 0x28
    out 0xA1, al

    mov al, 0x04
    out 0x21, al
    mov al, 0x02
    out 0xA1, al

    mov al, 0x01
    out 0x21, al
    out 0xA1, al

    mov al, 0xFD
    out 0x21, al
    mov al, 0xFF
    out 0xA1, al
    ret

init_keyboard:
    push eax
    mov al, 0xAE
    out 0x64, al
    mov al, 0xFF
    out 0x60, al
    pop eax
    ret

idt:
    times 256 * 8 db 0

idt_desc:
    dw 256 * 8 - 1
    dd idt

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

section .bss
resb 8192
stack_space: