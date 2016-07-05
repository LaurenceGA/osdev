#include "stdlib.h"


// Convert a value into its string representation.
char *itoa(int value, char *buff, int base) {
	const char digits[] = "0123456789abcdef";

	if (value < 0) {
		if (base == 10)
			*buff++ = '-';
		value *= -1;
	}

	int tmp = value;

	do {
		++buff;
		tmp /= base;
	} while (tmp > 0);

	*buff = '\0';

	do {
		*--buff = digits[value % base];
		value /= base;
	} while (value > 0);

	return buff;
}
