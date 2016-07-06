#include "string.h"

char *strrchr(const char *s, int c) {
	char *ss;

	for (ss = (char *) s + strlen(s); ss != s; ss--) {
		if (*ss == c)
			return ss;
	}

	return (*ss == c ? ss : (char *) NULL);
}
