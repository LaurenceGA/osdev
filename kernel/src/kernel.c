#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "tss.h"
#include "tty.h"
#include "gdt.h"
#include "idt.h"
#include "pic.h"

int main() {
	initTerminal();
	printf("Welcome to Kernel World!\n");
	printf("We've even got printf working!\n");

	uint32_t tss_vadr = tss_init();
	gdt_init(tss_vadr);

	pic_init();	// Re-map PIC
	init_idt();	// Load IDT

	printf("And we've reached the end.");

	return 0;
}
