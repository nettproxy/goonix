; boot.asm - multiboot !!

section .multiboot
    align 4
    dd 0x1BADB002            ; magic number
    dd 0x00                  ; flags
    dd -(0x1BADB002 + 0x00)  ; checksum

section .text
    global _start
    extern kernel_main
_start:
    mov esp, stack_top
    call kernel_main

    ; halt
    cli
    hlt

section .bss
    align 16
stack_bottom:
    resb 16384  ; 16 KB stack
stack_top: