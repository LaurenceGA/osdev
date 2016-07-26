#include "string.h"

/*
 * Set n characters of s to c.
 * Returns the updated void pointer that was passed.
 */
void *memset(void *s, int c, size_t n) {
	unsigned char *ss = (unsigned char *) s;

	for (size_t i = 0; i < n; i++)
		ss[i] = (unsigned char) c;

	return s;
}
