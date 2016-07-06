#include "string.h"

// Copies t into s
char *strcpy(char *s, const char *t) {
	char *ss = s;
	while ((*s++ = *t++))
		;
	return ss;
}

// Copies at most n chars from t into s, pad with '\0'
char *strncpy(char *s, const char *t, int n) {
	char *ss = s;
	while (n-- && (*s++ = *t++))
		;
	while (n-- > 0)		// Pad leftovers with '\0'
		*s++ = '\0';
	return ss;
}
