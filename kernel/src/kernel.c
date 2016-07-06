#include "tty.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

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
	printf("%c\n", *strchr("Hello", 0x65));
	printf("%c\n", *strrchr("Hello", 0x65));
	printf("%c\n", *strrchr("Hello", 0x48));
	printf("%d\n", atoi("10", 16));
	printf("%d\n", atoi("0x10", 16));
	printf("%d\n", atoi("10", 15));
	printf("%d\n", atoi("10", 14));
	printf("%d\n", atoi("10", 13));
	printf("%d\n", atoi("10", 12));
	printf("%d\n", atoi("10", 11));
	printf("%d\n", atoi("10", 10));
	printf("%d\n", atoi("10",  9));
	printf("%d\n", atoi("10",  8));
	printf("%d\n", atoi("\010",  8));
	printf("%d\n", atoi("10",  7));
	printf("%d\n", atoi("10",  6));
	printf("%d\n", atoi("10",  5));
	printf("%d\n", atoi("10",  4));
	printf("%d\n", atoi("10",  3));
	printf("%d\n", atoi("10",  2));
	printf("%d\n", atoi("0b10",  2));
	printf("%d\n", atoi("0B10",  2));

	return 0;
}
