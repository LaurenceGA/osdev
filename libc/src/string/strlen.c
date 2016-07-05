#include "string.h"

/**
 * Calculate the length of a null terminated string. The length is the number of
 * char's up to, but not including, the null terminator.
 */

size_t strlen(const char *string) {
	const char *ss;

	for (ss = string; *ss; ss++)
		;

	return ss - string;
}
