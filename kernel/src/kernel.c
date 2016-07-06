#include "tty.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int main() {
	initTerminal();
	printf("Welcome to Kernel World!\n");
	printf("%s\n", "We've even got printf working!");
	printf("Hello 0%c%x\n", 'x', 0x1af);
	char s[32] = "concat";
	printf("%s\n", strncat(s, "enated", 2));
	printf("%s\n", strcpy(s, "copied"));
	printf("%d\n", strcmp("aaaa", "aaab"));		// -1
	printf("%d\n", strcmp("aaaa", "aaaa"));		// 0
	printf("%d\n", strcmp("aaab", "aaaa"));		// 1
	printf("%d\n", strncmp("aaaac", "aaaab", 4));	// 0

	return 0;
}
