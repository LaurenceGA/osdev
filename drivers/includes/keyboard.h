#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "interrupts.h"

void initKBD();
unsigned char readScanCode();
unsigned char convertToASCII(unsigned char scanCode);
void KBDinterrupt(struct cpu_state cpu, unsigned int interrupt, struct stack_state stack);

#endif // KEYBOARD_H
