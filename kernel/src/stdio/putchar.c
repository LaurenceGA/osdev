#include "stdio.h"
#include "tty.h"

int putchar(int c) {
	char cc = (char) c;
	terminalPutC(cc);
	return cc;
}

