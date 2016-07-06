#include "stdlib.h"
#include "ctype.h"

// Convert a string representation of a number to it's base equivalent.
// If unsuccessful, return 0.
int atoi(char *str, int base) {
	// Only support bases between 2 and 16 for now.
	if (base < 2 && base > 36)
		return 0;

	int index, val = 0, tmp, sign = 1;

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
			else
				return 0;
			break;
		case 'x':
			if (base == 16)
				index += 2;
			else
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
