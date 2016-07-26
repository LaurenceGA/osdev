#include "string.h"

/*
 * Copy the contents of srcptr, of length n, into dstptr.
 */
void *memcpy(void *restrict dstptr, const void *restrict srcptr, size_t n) {
	unsigned char *dst = (unsigned char *) dstptr;
	const unsigned char *src = (const unsigned char *) srcptr;

	for (size_t i = 0; i < n; i++)
		dst[i] = src[i];

	return dstptr;
}
