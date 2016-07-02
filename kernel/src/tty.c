#include "string.h"
#include "tty.h"
#include "vga.h"

size_t terminal_row;
size_t terminal_column;

uint8_t terminal_colour;
uint16_t *terminal_buffer;

void initTerminal() {
	terminal_row    = 0;
	terminal_column = 0;

	terminal_colour = makeColour(COLOUR_WHITE, COLOUR_BLACK);
	terminal_buffer = VIDEO_MEMORY;

	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = makeVGAEntry(' ', terminal_colour);
		}
	}
}

void terminalSetColour(uint8_t colour) {
        terminal_colour = colour;
}

void terminalMvPutC(char c, uint8_t colour, size_t col, size_t row) {
        const size_t index = row * VGA_WIDTH + col;
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
	size_t len = strlen(str);
	for (size_t i = 0; i < len; i++)
		terminalPutC(str[i]);
}

