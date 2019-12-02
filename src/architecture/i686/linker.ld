/* start of text */
ENTRY(_start)

SECTIONS
{
	. = 1M;

	.text ALIGN(4K) :
	{
		*(.multiboot)
		*(.text)

		/*
        *(SORT(.ctors.*))
        *(.ctors)

        *(SORT(.dtors.*))
        *(.dtors)
		*/
	}

	/*
	.init ALIGN(4K) :
	{
		*(.init)
	}

	.fini ALIGN(4k) :
	{
		*(.fini)
	}
	*/

	.data ALIGN(4K) :
	{
		*(.data)
	}

	.bss :
	{
		*(.bss)
		*(.stack)
	}

	/* other sections */
}