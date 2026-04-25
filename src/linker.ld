ENTRY(_start)

SECTIONS
{
    . = 1M;
    kernel_start = .;

    /* Multiboot header MUST be first */
    .multiboot ALIGN(4) : {
        *(.multiboot)
    }

    .text ALIGN(4) : {
        *(.text*)
    }

    .rodata ALIGN(4) : {
        *(.rodata*)
    }

    .data ALIGN(4) : {
        *(.data*)
    }

    .bss ALIGN(4) : {
        *(COMMON)
        *(.bss*)
    }
    kernel_end = .;

    /DISCARD/ : {
        *(.comment)
        *(.eh_frame)
    }
}
