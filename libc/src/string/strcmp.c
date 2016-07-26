#include "string.h"

// Return < 0 if s1 < t, 0 if s==t, >0 if s>t
int strcmp(const char *s1, const char *s2) {
	for ( ; *s1 == *s2; s1++, s2++) {
		if (!*s1)
			return 0;
	}

	return *s1 - *s2;
}

// Return <0 if s<t, 0 if s==t, >0 if s>t. max n chars
int strncmp(const char *s, const char *s2, size_t n) {
	for ( ; *s == *s2 && n--; s++, s2++) {
		if (!*s || !n)	// End of string or same until n chars in
			return 0;
	}

	return *s - *s2;
}
