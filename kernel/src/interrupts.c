#include "interrupts.h"
#include "stdio.h"
#include "pic.h"
#include "io.h"

#define KBD_DATA_PORT 0x60

unsigned char readScanCode() {
	return inb(KBD_DATA_PORT);
}

void KBDinterrupt(struct cpu_state cpu, unsigned int interrupt,
			struct stack_state stack) {
	unsigned char scanCode = readScanCode();
	printf("Key event! (SC=%#x)\n", scanCode);
	// Arguments unused - this supresses warning
	(void) interrupt;
	(void) cpu;
	(void) stack;
}

void interrupt_handler(struct cpu_state cpu, unsigned int interrupt,
			struct stack_state stack) {
	// printf("Interrupt %#x called!\n", interrupt);
	if (interrupt == KBD_INT_IDX) {
		KBDinterrupt(cpu, interrupt, stack);
	}
	// if (cpu.eax && stack.error_code){};
	pic_acknowledge();
}
