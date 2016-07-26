#include "string.h"

// Copies s2 into s1.
char *strcpy(char *restrict s1, const char *restrict s2) {
	char *ss = s1;

	while ((*s1++ = *s2++))
	/* Do Nothing */;

	return ss;
}

// Copies at most n chars from s2 into s1, pad with '\0'.
char *strncpy(char *restrict s1, const char *restrict s2, size_t n) {
	char *ss = s1;

	while (n-- && (*s1++ = *s2++))
	/* Do Nothing */;

	while (n-- > 0)		// Pad leftovers with '\0'.
		*s1++ = '\0';
	return ss;
}
