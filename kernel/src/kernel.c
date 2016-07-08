#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "tty.h"
#include "idt.h"
#include "pic.h"

int main() {
	initTerminal();
	printf("Welcome to Kernel World!\n");
	printf("We've even got printf working!\n");

	pic_init();	// Re-map PIC
	init_idt();	// Load IDT

	return 0;
}
