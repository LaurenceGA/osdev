#include "string.h"

/*
 * Find the first occurence of c in the string s.
 *
 * Returns a pointer to c in the string if it was found, otherwise NULL.
 */
char *strchr(const char *s, int c) {
	char *ss;

	for (ss = (char *) s; *ss; ss++) {
		if (*ss == c)
			return ss;
	}

	// The value we wanted to find was 0, which is at the end of every string.
	if (!c)
		return ss;

	return (char *) NULL;
}

/*
 * Find the last occurence of c in the string s.
 *
 * Returns a pointer to c in the string if it was found, otherwise NULL.
 */
char *strrchr(const char *s, int c) {
	char *ss;

	for (ss = (char *) s + strlen(s); ss >= s; ss--) {
		if (*ss == c)
			return ss;
	}

	return (char *) NULL;
}
