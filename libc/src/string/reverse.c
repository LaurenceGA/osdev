#include "string.h"

/*
 * Reverse the string s, and then return it.
 */
char *reverse(char *s) {
	for (int i = 0, x = strlen(s) - 1, y; i < x; i++, x--) {
		y = s[i];
		s[i] = s[x];
		s[x] = y;
	}

	return s;
}
