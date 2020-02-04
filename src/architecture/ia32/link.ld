ENTRY(_start)

SECTIONS
{
    . = 0x00100000;

/*
    .multiboot ALIGN(4)		:
    {
    }
*/
    .init	:
    {
        KEEP(*(.multiboot))
        KEEP(*(SORT_NONE(.init)))
    }

    .text	:
    {
        *(.text)
        *(.text.*)
    }

    PROVIDE(_etext = .);

    .fini	 :
    {
        KEEP(*(SORT_NONE(.fini)))
    }

    .init_array		:
    {
        /* global constructors */
        KEEP(crtbegin.o(.init_array))
        KEEP(*(.ctors))
        KEEP(crtend.o(.init_array))
    }

    .fini_array		:
    {
        /* global destructors */
        KEEP(crtbegin.o(.fini_array))
        KEEP(*(.dtors))
        KEEP(crtend.o(.fini_array))
    }

    .data	:
    {
        *(.data)
        /* collect inline variables */
        *(.data.*)
    }

    PROVIDE(_edata = .);

    .bss	:
    {
        *(.bss)
        /* collect inline variables */
        *(.bss.*)
    }

    PROVIDE(_end = .);

    /DISCARD/	:
    {
        /* discard exception handling stuff */
        *(.comment)
        *(.eh_frame)
    }
}