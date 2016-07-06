#include "tty.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int main() {
	initTerminal();
	printf("Welcome to Kernel World!\n");
	printf("%s%%\n", "We've even got printf working!");
	printf("Hello %0#6x\n", 0x1af);
	char s[32] = "concat";
	printf("%s\n", strncat(s, "enated", 2));
	printf("%0+5d\n", -12);
	printf("%d\n", atoi("10", 36));

	return 0;
}
