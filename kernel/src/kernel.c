#include "tty.h"
#include "stdio.h"

int main() {
	initTerminal();
	printf("Welcome to Kernel World!\n");
	printf("%s\n", "We've even got printf working!");
	printf("%cHello\n", 'd');

	ttySetCursor(0);

	return 0;
}
