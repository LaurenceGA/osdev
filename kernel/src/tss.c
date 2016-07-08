#include "tss.h"

static struct tss _tss;

uint32_t tss_init() {
	return (uint32_t) &_tss;
}

void tss_set_kernel_stack(uint16_t segsel, uint32_t vaddr) {
	_tss.esp0 = vaddr;
	_tss.ss0 = segsel;
}
