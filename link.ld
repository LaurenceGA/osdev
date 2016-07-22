
ENTRY(_loader)

SECTIONS {
    /* The kernel resides at 3GB + 1MB in the virtual address space. This is
       mapped to 1MB in the physical address space. */
    . = 0xC0100000;

    .text : AT(ADDR(.text) - 0xC0000000) {
        *(.text)
        *(.rodata*)
    }

    .data ALIGN (0x1000) : AT(ADDR(.data) - 0xC0000000) {
        *(.data)
    }

    .bss : AT(ADDR(.bss) - 0xC0000000) {
        _sbss = .;
	*(COMMON)
	*(.bss)
	_ebss = .;
    }
}
