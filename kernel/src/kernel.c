#include "include/tty.h"

int main(void) {
	char* videoMemory = (char*) 0xb8000;

	*videoMemory = 'X';

	ttyMoveCursor(6);

	return 0;
}
