#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "tss.h"
#include "tty.h"
#include "gdt.h"
#include "idt.h"
#include "pic.h"
#include "keyboard.h"
#include "multiboot.h"
#include "constants.h"

static multiboot_info_t *remapMBI(uint32_t mbaddr) {
    multiboot_info_t *mbinfo = (multiboot_info_t *) PHYSICAL_TO_VIRT_ADDR(mbaddr);

    mbinfo->mmap_addr = PHYSICAL_TO_VIRT_ADDR(mbinfo->mmap_addr);
    mbinfo->mods_addr = PHYSICAL_TO_VIRT_ADDR(mbinfo->mods_addr);

    return mbinfo;
}

void kernel_virtual_end(void);
void kernel_virtual_start(void);
void kernel_physical_end(void);
void kernel_physical_start(void);

int kmain(unsigned int mbt_addr, unsigned int magic,
	unsigned int kernelPDT, unsigned int kernelPT) {

	initTerminal();
	printf("Welcome to Kernel World!\n");
	printf("We've even got printf working!\n");

	uint32_t tss_vadr = initTSS();
	initGDT(tss_vadr);

	initPIC();	// Re-map PIC
	initIDT();	// Load IDT
	initKBD();

	printf("And we've reached the end.\n");

	multiboot_info_t *mbti = remapMBI(mbt_addr);

	// mbti->mem_upper and mbti->mem_lower are in KiB (1024 bytes).
	printf("\nWe have %d MiB of usable Memory!\n",
		(mbti->mem_upper + mbti->mem_lower) >> 10);

	unsigned long TotalMapAddress = mbti->mmap_addr;
	unsigned long TotalMapLength = mbti->mmap_length;

	printf("%#x\n", TotalMapAddress);
	printf("%#x\n", TotalMapLength);

	memory_map_t *mmap = (memory_map_t *) mbti->mmap_addr;

	while((unsigned long) mmap < TotalMapAddress + TotalMapLength){
		printf("%#x -- %d == %#x == %#x\n", mmap, mmap->type,
			mmap->base_addr_low, mmap->length_low);

		if(mmap->type == 1) {
                	//UsableMemoryEnd = CurrentReadingAddress + MemMap->size;
		}
		//CurrentReadingAddress += MemMap->size;
		mmap = (memory_map_t *) ((unsigned long) mmap + mmap->size + sizeof mmap->size);
        }

	return 0;
}
