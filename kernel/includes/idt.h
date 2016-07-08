#ifndef IDT_H
#define IDT_H

// const unsigned int NUM_IDT_ENTRIES = 256;
#define NUM_IDT_ENTRIES 256

// Takes care of all the interrupt descriptor table (IDT) initialization
void init_idt();

#endif // IDT_H
