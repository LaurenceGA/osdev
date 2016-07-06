#include "string.h"

// concatenate t to the end of s; s must be big enough. Returns s
char *strcat(char *s, const char *t) {
	char *ss = s;	// So we can return our string
	while (*s)	// Find the end of s
		s++;
	while ((*s++ = *t++))
		;
	return ss;
}

// concatenate max n chars of t to the end of s; s must be big enough, Returns s
char *strncat(char *s, const char *t, int n) {
	char *ss = s;	// So we can return our string
	while (*s)	// Find the end of s
		s++;
	strncpy(s, t, n);
	return ss;
}
