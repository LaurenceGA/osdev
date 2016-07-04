#include "tty.h"
#include "stdio.h"

int main() {
	initTerminal();
	printf("Welcome to Kernel World!\n");
	printf("%s\n", "We've even got printf working!");
	printf("%cHello\n", "Cd");

	return 0;
}
