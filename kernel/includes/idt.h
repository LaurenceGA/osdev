#ifndef IDT_H
#define IDT_H

#define NUM_IDT_ENTRIES 256

// Takes care of all the interrupt descriptor table (IDT) initialization
void initIDT();

#endif // IDT_H
