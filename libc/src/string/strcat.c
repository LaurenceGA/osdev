#include "string.h"

// concatenate t to the end of s; s must be big enough. Returns s
char *strcat(char *restrict s, const char *restrict t) {
	char *ss = s;	// So we can return our string
	while (*s)	// Find the end of s
		s++;
	while ((*s++ = *t++))
	/* Do nothing */;
	return ss;
}

// concatenate max n chars of t to the end of s; s must be big enough, Returns s
char *strncat(char *restrict s, const char *restrict t, size_t n) {
	char *ss = s;	// So we can return our string
	while (*s)	// Find the end of s
		s++;
	strncpy(s, t, n);
	return ss;
}
