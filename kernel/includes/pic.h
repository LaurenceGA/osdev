#ifndef PIC_H
#define PIC_H

#define PIC1_START      0x20
#define PIC2_START      0x28
#define PIC_NUM_IRQS    16

#define PIT_INT_IDX     PIC1_START
#define KBD_INT_IDX     PIC1_START + 1

#define COM1_INT_IDX    PIC1_START + 4
#define COM2_INT_IDX    PIC1_START + 3

void initPIC();
void pic_acknowledge();
void pic_mask(unsigned char mask1, unsigned char mask2);

#endif // PIC_H
