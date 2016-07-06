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
	printf("%#x\n", 5);
	printf("%#x\n", 0xd);
	printf("%#x\n", 0x45f);
	printf("%d\n", 5);
	printf("%d\n", -12);
	printf("%d\n", 243);
	printf("%d\n", 0x100);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	printf("Just checking to see where this ends up and if it continues on to the next line or not.\n");

	return 0;
}
