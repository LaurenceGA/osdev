#include "include/tty.h"

int main(void) {
	char* videoMemory = (char*) 0xb8000;

	*videoMemory = 'X';

	ttySetCursor(0);

	return 0;
}
