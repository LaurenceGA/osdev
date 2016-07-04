#include "string.h"
#include "tty.h"
#include "vga.h"

// The current row and column we're on.
int terminal_row;
int terminal_column;

// The current colour of the output.
uint8_t terminal_colour;
// An array to hold all of the output characters. It starts at address VIDEO_MEMORY
uint16_t *terminal_buffer;

// Set the foreground and background of all output starting when this function
// is called
static uint8_t makeColour(enum COLOUR fg, enum COLOUR bg) {
	return fg | bg << 4;
}

// We want a 16 bit value to put to screen, with the top half being the background
// and foreground colours, and the bottom 8 bits being the character.
static uint16_t makeVGAEntry(char c, uint8_t colour) {
	uint16_t c16 = c;
	uint16_t colour16 = colour;
	return c16 | colour16 << 8;
}

// Initilise the terminal we're going to draw to.
void initTerminal() {
	terminal_row    = 0;
	terminal_column = 0;

	terminal_colour = makeColour(COLOUR_WHITE, COLOUR_BLACK);
	terminal_buffer = VIDEO_MEMORY;

	for (int y = 0; y < VGA_HEIGHT; y++) {
		for (int x = 0; x < VGA_WIDTH; x++) {
			const int index = y * VGA_WIDTH + x;
			terminal_buffer[index] = makeVGAEntry(' ', terminal_colour);
		}
	}
}

// Change the colour of the output from here on.
void terminalSetColour(uint8_t colour) {
	terminal_colour = colour;
}

// Move to a specific row and column before drawing the character.
void terminalMvPutC(char c, uint8_t colour, int col, int row) {
	const int index = row * VGA_WIDTH + col;
	terminal_buffer[index] = makeVGAEntry(c, colour);
}

// Put a charcter to the screen.
void terminalPutC(char c) {
	if (c == '\n') {
		terminal_column = 0;
		goto down_row;
	} else if (c == '\r') {
		terminal_column = 0;
		return;
	} else
		terminalMvPutC(c, terminal_colour, terminal_column, terminal_row);

	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
down_row:
		if (++terminal_row == VGA_HEIGHT)
			terminal_row = 0;
	}
}

// Put a string to the screen.
void terminalPutS(const char *str) {
	int len = strlen(str);
	for (int i = 0; i < len; i++)
		terminalPutC(str[i]);
}
