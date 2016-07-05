#include "string.h"

void reverse(char *str) {
	int i, x, y;

	for (i = 0, x = strlen(str) - 1; i < x; i++, x--) {
		y = str[i];
		str[i] = str[x];
		str[x] = y;
	}
}
