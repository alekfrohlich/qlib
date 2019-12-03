/* start of text */
ENTRY(_start)

SECTIONS
{
	. = 1M;

	.text ALIGN(4K) :
	{
		*(.multiboot)
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

	__edata = .;

	/DISCARD/ : {
		*(.comment)
	}
}