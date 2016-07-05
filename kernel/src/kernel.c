#include "tty.h"
#include "stdio.h"
#include "stdlib.h"

int main() {
	initTerminal();
	printf("Welcome to Kernel World!\n");
	printf("%s\n", "We've even got printf working!");
	printf("Hello %c\n", 'x');
	char c[30];
	itoa(-32000, c, 16);
	printf("%s\n", c);

	return 0;
}
