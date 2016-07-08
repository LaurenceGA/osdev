#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "tty.h"
#include "idt.h"
#include "pic.h"
#include "keyboard.h"

int main() {
	initTerminal();
	printf("Welcome to Kernel World!\n");
	printf("We've even got printf working!\n");

	initPIC();	// Re-map PIC
	initKBD();
	initIDT();	// Load IDT

	return 0;
}
