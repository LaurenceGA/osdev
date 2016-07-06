#include "string.h"

char *strchr(const char *s, int c) {
	char *ss;

	for (ss = (char *) s; *ss ;ss++) {
		if (*ss == c)
			return ss;
	}

	return (c == 0 ? ss : (char *) NULL);
}
