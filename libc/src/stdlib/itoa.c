#include "stdlib.h"
#include "string.h"


// Convert a value into its string representation.
char *itoa(int value, char *buff, int base) {
	if (base < 2 || base > 36) {
		*buff = '\0';
		return buff;
	}

	const char digits[] = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz";
	char *ctmp = buff;

	if (value < 0 && base == 10) {
		int tmp;

		do {
			tmp = value;
			value /= base;
			*ctmp++ = digits[35 + (tmp - value * base)];
		} while (value);

		*ctmp++ = '-';
	} else {
		unsigned int unsigned_equiv = (unsigned int) value;
		unsigned int utmp;

		do {
			utmp = unsigned_equiv;
			unsigned_equiv /= base;
			*ctmp++ = digits[35 + (utmp - unsigned_equiv * base)];
		} while (unsigned_equiv);
	}

	*ctmp = '\0';

	reverse(buff);

	return buff;
}
