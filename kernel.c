
char *VIDEO_MEMORY = (char *) 0xb8000;

int main() {
        *VIDEO_MEMORY = 'X';

        return 0;
}
