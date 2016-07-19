#include "stdlib.h"
#include "ctype.h"

// Convert a string representation of a number to it's base equivalent.
// If unsuccessful, return 0.
long atol(const char *str, int base) {
	// Only support bases between 2 and 16 for now.
	if (base > 36 || base < 2)
		return 0;

	long val = 0, tmp, sign = 1;
	int index;

	for (index = 0; isspace(str[index]); index++)
		;

	if (str[index] == '+' || str[index] == '-') {
		if (base == 10)
			sign = (str[index++] == '-') ? -1 : 1;
		else
			return 0;
	} else if (str[index] == '0') {
		switch(str[index + 1]) {
		case 'b':
		case 'B':
			if (base == 2)
				index += 2;
			else if (base < 12)
				return 0;
			break;
		case 'x':
		case 'X':
			if (base == 16)
				index += 2;
			else if (base < 34)
				return 0;
			break;
		default:
			break;
		}
	} else if (str[index] == '\0' && str[index + 1] >= 0x30 && str[index + 1] <= 0x37) {
		if (base == 8)
			index++;
		else
			return 0;
	}

	for (tmp = 0; isalnum(str[index]); index++) {
		tmp = str[index] - '0';
		if (base <= 10) {
			if (tmp >= base)
				return 0;
		} else if ((tmp -= islower(str[index]) ? 0x27 : 0x7) >= base) {
			return 0;
		}
		val = val * base + tmp;
	}

	if (base == 10)
		return sign * val;
	else
		return val;
}
