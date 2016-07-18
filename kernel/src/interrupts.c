#include "interrupts.h"
#include "idt.h"
#include "pic.h"

static interrupt_handler_func handlers[NUM_IDT_ENTRIES];

void registerInterruptHandler(unsigned int interrupt, interrupt_handler_func func) {
	handlers[interrupt] = func;
}

void interrupt_handler(struct cpu_state cpu, unsigned int interrupt,
			struct stack_state stack) {
	if (handlers[interrupt] != 0) {
		handlers[interrupt](cpu, interrupt, stack);
	}
	pic_acknowledge();
}
