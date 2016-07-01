int main(void) {
	char* videoMemory = (char*) 0xb8000;

	*videoMemory = 'X';

	return 0;
}
