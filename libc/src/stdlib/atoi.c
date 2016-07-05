#include "stdlib.h"
#include "ctype.h"

int atoi(char *str) {
	int index, tmp, sign;

	for (index = 0; isspace(str[index]); index++)
		;

	sign = (str[index] == '-') ? -1 : 1;

	if (str[index] == '+' || str[index] == '-')
		index++;

	for (tmp = 0; isdigit(str[index]); index++)
		tmp = tmp * 10 + (str[index] - '0');

	return sign * tmp;
}
