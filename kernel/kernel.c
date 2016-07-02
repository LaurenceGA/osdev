#include <stddef.h>
#include <stdint.h>

static uint16_t *VIDEO_MEMORY = (uint16_t *) 0xB8000;
static const size_t VGA_HEIGHT = 25;
static const size_t VGA_WIDTH  = 80;

enum COLOUR {
	COLOUR_BLACK		= 0,
	COLOUR_BLUE		= 1,
	COLOUR_GREEN		= 2,
	COLOUR_CYAN		= 3,
	COLOUR_RED		= 4,
	COLOUR_MAGENTA		= 5,
	COLOUR_BROWN		= 6,
	COLOUR_LIGHT_GREY	= 7,
	COLOUR_DARK_GREY	= 8,
	COLOUR_LIGHT_BLUE	= 9,
	COLOUR_LIGHT_GREEN	= 10,
	COLOUR_LIGHT_CYAN	= 11,
	COLOUR_LIGHT_RED	= 12,
	COLOUR_LIGHT_MAGENTA	= 13,
	COLOUR_LIGHT_BROWN	= 14,
	COLOUR_WHITE		= 15,
};
 

size_t terminal_row;
size_t terminal_column;

uint8_t terminal_colour;
uint16_t *terminal_buffer;

size_t strlen(const char *string) {
	size_t len = 0;
	while (string[len])
		len++;
	return len;
}

uint8_t make_colour(enum COLOUR fg, enum COLOUR bg) {
	return fg | bg << 4;
}

uint16_t make_vga_entry(char c, uint8_t colour) {
        uint16_t c16 = c;
        uint16_t colour16 = colour;
        return c16 | colour16 << 8;
}

void init_terminal() {
	terminal_row    = 0;
	terminal_column = 0;

	terminal_colour = make_colour(COLOUR_WHITE, COLOUR_BLACK);
	terminal_buffer = VIDEO_MEMORY;

	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = make_vga_entry(' ', terminal_colour);
		}
	}
}

void terminal_setcolour(uint8_t colour) {
        terminal_colour = colour;
}

void terminal_mvputc(char c, uint8_t colour, size_t col, size_t row) {
        const size_t index = row * VGA_WIDTH + col;
        terminal_buffer[index] = make_vga_entry(c, colour);
}

void terminal_putc(char c) {
	if (c == '\n') {
		goto down_row;
	} else if (c == '\r') {
		terminal_column = 0;
		return;
	} else
		terminal_mvputc(c, terminal_colour, terminal_column, terminal_row);

	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
down_row:
		if (++terminal_row == VGA_HEIGHT)
			terminal_row = 0;
        }
}

void terminal_puts(const char *str) {
	size_t len = strlen(str);
	for (size_t i = 0; i < len; i++)
		terminal_putc(str[i]);
}

int main() {
	init_terminal();
	terminal_puts("Welcome to kernel world!\n");
	terminal_puts("Here we can do all sorts\n\rof fun things");

	return 0;
}
