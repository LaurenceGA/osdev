#ifndef INTERRUPTS_H
#define INTERRUPTS_H

struct cpu_state {
	unsigned int edi;
	unsigned int esi;
	unsigned int ebp;
	unsigned int edx;
	unsigned int ecx;
	unsigned int ebx;
	unsigned int eax;
	unsigned int esp;
} __attribute__((packed));

struct stack_state {
	unsigned int error_code;
	unsigned int eip;
	unsigned int cs;
	unsigned int eflags;
} __attribute__((packed));

typedef void (*interrupt_handler_func)(struct cpu_state cpu, unsigned int interrupt, struct stack_state stack);
void interrupt_handler(struct cpu_state cpu, unsigned int interrupt, struct stack_state stack);
void registerInterruptHandler(unsigned int interrupt, interrupt_handler_func func);

#endif // INTERRUPTS_H
