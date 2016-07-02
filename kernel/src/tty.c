#include "string.h"
#include "tty.h"
#include "vga.h"

int terminal_row;
int terminal_column;

uint8_t terminal_colour;
uint16_t *terminal_buffer;

static uint8_t makeColour(enum COLOUR fg, enum COLOUR bg) {
	return fg | bg << 4;
}

static uint16_t makeVGAEntry(char c, uint8_t colour) {
	uint16_t c16 = c;
	uint16_t colour16 = colour;
	return c16 | colour16 << 8;
}

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

void terminalSetColour(uint8_t colour) {
	terminal_colour = colour;
}

void terminalMvPutC(char c, uint8_t colour, int col, int row) {
	const int index = row * VGA_WIDTH + col;
	terminal_buffer[index] = makeVGAEntry(c, colour);
}

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

void terminalPutS(const char *str) {
	int len = strlen(str);
	for (int i = 0; i < len; i++)
		terminalPutC(str[i]);
}

