#include "string.h"

// Return <0 if s<t, 0 if s==t, >0 if s>t
int strcmp(const char *s, const char *t) {
	for ( ; *s == *t; s++, t++)
		if (!*s)
			return 0;
	return *s - *t;
}

// Return <0 if s<t, 0 if s==t, >0 if s>t. max n chars
int strncmp(const char *s, const char *t, int n) {
	// n--;	//
	for ( ; *s == *t && n--; s++, t++) {
		if (!*s || !n)	// End of string or same until n chars in
			return 0;
	}
	return *s - *t;
}
