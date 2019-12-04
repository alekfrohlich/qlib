/* start of text */
ENTRY(_start)

SECTIONS
{
	. = 1M;

	.multiboot ALIGN(4):
	{
		*(.multiboot)
	}

	.init :
  	{
    	KEEP(*(SORT_NONE(.init)))
  	}

	.text :
	{
		*(.text)
	}

	.fini :
	{
		KEEP(*(SORT_NONE(.fini)))
	}

	.init_array :
	{
	  KEEP(crtbegin.o(.init_array))
      KEEP(*(.ctors))
	  KEEP(crtend.o(.init_array))
	}

	.fini_array     :
	{
	  KEEP(crtbegin.o(.fini_array))
      KEEP(*(.dtors))
	  KEEP(crtend.o(.fini_array))
	}

	.data :
	{
		*(.data)
	}

	.bss :
	{
		*(.bss)
	}

	. = ALIGN(16);
	PROVIDE(_edata = .);

	/DISCARD/ : {
		*(.comment)
		*(.eh_frame)
	}
}