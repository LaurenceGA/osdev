#include "string.h"

void *memmove(void *dstptr, const void *srcptr, size_t n) {
	unsigned char *dst = (unsigned char *) dstptr;
	const unsigned char *src = (const unsigned char *) srcptr;

	if (dst < src) {
		for (size_t i = 0; i < n; ++i)
			dst[i] = src[i];
	} else {
		for (size_t i = n; i; --i)
			dst[i - 1] = src[i - 1];
	}

	return dstptr;
}
