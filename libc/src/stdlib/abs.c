#include "stdlib.h"

int abs(int j) {
	if (j < 0)
		j *= -1;
	return j;
}

long labs(long j) {
	if (j < 0)
		j *= -1;
	return j;
}
