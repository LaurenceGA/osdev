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

typedef void (*interruptHandlerFunc)(struct cpu_state cpu, unsigned int interrupt, struct stack_state stack);
void interruptHandler(struct cpu_state cpu, unsigned int interrupt, struct stack_state stack);
void registerInterruptHandler(unsigned int interrupt, interruptHandlerFunc func);

void disableInterrupts(void);

#endif // INTERRUPTS_H
