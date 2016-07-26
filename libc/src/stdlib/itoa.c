#include "stdlib.h"
#include "string.h"

/*
 * Convert the value into it's corresponding string representation, respecting
 * the base that is passed.
 *
 * Returns the string representation if successful, otherwise a string of length 0.
 */
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

		// The do loop is needed in case the value is 0.
		do {
			utmp = unsigned_equiv;
			unsigned_equiv /= base;
			*ctmp++ = digits[35 + (utmp - unsigned_equiv * base)];
		} while (unsigned_equiv);
	}

	*ctmp = '\0';

	return reverse(buff);
}
