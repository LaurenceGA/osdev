#include "string.h"

void *memchr(const void *s, int c, size_t n) {
	unsigned char ch = (unsigned char) c;

	for (size_t index = 0; index < n; ++index) {
		if (*((unsigned char *) s + index) == ch)
			return (void *) ((unsigned char *) s + index);
	}

	return NULL;
}
