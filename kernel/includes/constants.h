#ifndef _KCONSTANTS_H
#define _KCONSTANTS_H

// Virtual Memory stuff
#define KERNEL_START_VADDR 	0xC0000000
#define KERNEL_PDT_IDX		(KERNEL_START_VADDR >> 22)

#define PHYSICAL_TO_VIRT_ADDR(addr) ((addr)+KERNEL_START_VADDR)

#endif // _KCONSTANTS_H
