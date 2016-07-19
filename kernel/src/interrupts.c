#include "interrupts.h"
#include "idt.h"
#include "pic.h"

static interruptHandlerFunc handlers[NUM_IDT_ENTRIES];

void registerInterruptHandler(unsigned int interrupt, interruptHandlerFunc func) {
	handlers[interrupt] = func;
}

void interruptHandler(struct cpu_state cpu, unsigned int interrupt,
			struct stack_state stack) {
	if (handlers[interrupt] != 0) {
		handlers[interrupt](cpu, interrupt, stack);
	}
	pic_acknowledge();
}
