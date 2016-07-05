#include "stdlib.h"


// Convert a value into its string representation.
char *itoa(int value, char *buff, int base) {
	const char digits[] = "0123456789abcdef";
	char *s = buff;

	if (value < 0 && base == 10) {
		*s++ = '-';
		value *= -1;
	}

	int tmp = value;

	do {
		++s;
		tmp /= base;
	} while (tmp > 0);

	*s = '\0';

	do {
		*--s = digits[value % base];
		value /= base;
	} while (value > 0);

	return buff;
}
