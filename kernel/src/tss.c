#include "tss.h"

static struct tss _tss;

uint32_t initTSS(void) {
	return (uint32_t) &_tss;
}

void TSSSetKernelStack(uint16_t segsel, uint32_t vaddr) {
	_tss.esp0 = vaddr;
	_tss.ss0 = segsel;
}
