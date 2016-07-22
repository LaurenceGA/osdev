#include "string.h"
#include "stddef.h"

char *strstr(const char *s1, const char *s2) {
	size_t len_s1, len_s2;

	len_s2 = strlen(s2);
	if (!len_s2) return (char *)s1;

	for (len_s1 = strlen(s1); len_s1 >= len_s2; --len_s1, ++s1) {
		if (!memcmp(s1, s2, len_s2))
			return (char *)s1;
	}

	return NULL;
}
