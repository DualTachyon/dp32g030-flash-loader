ENTRY(Init)

MEMORY
{
	RAM (xrw) : ORIGIN = 0x00000000, LENGTH = 1K
}

SECTIONS
{
	PrgCode :
	{
		. = ALIGN(4);
		*(.PrgCode)
		*(.PrgCode*)
		*(.text)
		*(.text*)
		. = ALIGN(4);
	} >RAM

	PrgData :
	{
		. = ALIGN(4);
		*(.PrgData)
		*(.PrgData*)
		*(.data)
		*(.data*)
		*(.rodata)
		*(.rodata*)
		_sbss = .;
		*(.bss)
		*(.bss*)
		. = ALIGN(4);
	} >RAM

	DevDscr :
	{
		. = ALIGN(4);
		*(.DevDscr)
		*(.DevDscr*)
		*(.constdata)
		*(.constdata*)
		. = ALIGN(4);
	} >RAM

	/DISCARD/ :
	{
		libc.a ( * )
		libm.a ( * )
		libgcc.a ( * )
	}
}
