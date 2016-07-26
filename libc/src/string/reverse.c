#include "string.h"

/*
 * Reverse the string s.
 */
void reverse(char *s) {
	int y;

	for (int i = 0, x = strlen(s) - 1; i < x; i++, x--) {
		y = s[i];
		s[i] = s[x];
		s[x] = y;
	}
}
