#include "tty.h"
#include "stdio.h"
#include "io.h"

int main() {
	initTerminal();
	printf("Welcome to Kernel World!\n");
	printf("%s\n", "We've even got printf working!");
	printf("Hello %c\n", 'x');

	return 0;
}
