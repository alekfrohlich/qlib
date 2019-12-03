/* start of text */
ENTRY(_start)

SECTIONS
{
	. = 1M;

	.multiboot ALIGN(4) :
	{
		*(.multiboot)
	}

	.text ALIGN(4K) :
	{
		*(.text)
	}

	.data ALIGN(4K) :
	{
		*(.data)
	}

	.bss :
	{
		*(.bss)
	}

	edata = .;

	/DISCARD/ : {
		*(.comment)
	}
}