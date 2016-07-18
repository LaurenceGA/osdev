#include "string.h"
#include "tty.h"
#include "io.h"

uint16_t *const VIDEO_MEMORY = (uint16_t *) 0xB8000;
const size_t VGA_HEIGHT = 25;
const size_t VGA_WIDTH  = 80;

// The current row and column we're on.
int terminal_row;
int terminal_column;

// The current colour of the output.
uint8_t terminal_colour;
// An array to hold all of the output characters. It starts at address VIDEO_MEMORY
uint16_t *terminal_buffer;

// I/O ports
const unsigned short TTY_COMMAND_PORT = 0x3d4;
const unsigned short TTY_DATA_PORT = 0x3d5;

// I/O port commands
const unsigned char TTY_HIGH_BYTE_CMD = 14;
const unsigned char TTY_LOW_BYTE_CMD = 15;

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

	clearTerminal();
}

void clearTerminal() {
	for (unsigned int y = 0; y < VGA_HEIGHT; y++) {
		for (unsigned int x = 0; x < VGA_WIDTH; x++) {
			unsigned int index = getCharOffset(x, y);
			terminal_buffer[index] = makeVGAEntry(' ', terminal_colour);
		}
	}
}

unsigned int getCharOffset(unsigned int x, unsigned int y) {
	return y * VGA_WIDTH + x;
}

// Shuffle each terminal row down one, and blank the last row.
void doScroll() {
	// Move each row down (up, on the screen) one.
	for (unsigned int y = 1; y < VGA_HEIGHT; y ++) {
		memcpy(terminal_buffer + getCharOffset(0, y - 1),
			terminal_buffer + getCharOffset(0, y), VGA_WIDTH * 2);
	}

	// Empty the last row.
	for (unsigned int x = 0; x < VGA_WIDTH * 2; x++) {
		unsigned int index = getCharOffset(x, VGA_HEIGHT - 1);
		terminal_buffer[index] = makeVGAEntry(0, terminal_colour);
	}
}

// Check if the terminal needs to be scrolled.
void checkScroll() {
	// Increment the current column because we just printed a character.
	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		++terminal_row;
	}

	if (terminal_row == VGA_HEIGHT) {
		doScroll();
		// Go back to the last row.
		--terminal_row;
	}
}

// Change the colour of the output from here on.
void terminalSetColour(uint8_t colour) {
	terminal_colour = colour;
}

// Move to a specific row and column before drawing the character.
void terminalMvPutC(char c, uint8_t colour, int col, int row) {
	const int index = getCharOffset(col, row);
	terminal_buffer[index] = makeVGAEntry(c, colour);
}

// Put a charcter to the screen.
void terminalPutC(char c) {
	switch (c) {
		case '\n':
			++terminal_row;
			checkScroll();
			// FALLTRHOUGH
		case '\r':
			terminal_column = 0;
			break;
		case '\b':
		{
			unsigned short cursorPos = ttyGetCursor();
			if (cursorPos)
				--cursorPos;
			terminal_column = cursorPos % VGA_WIDTH;
			terminal_row = cursorPos / VGA_WIDTH;
			break;
		}
		default:
			terminalMvPutC(c, terminal_colour, terminal_column,
					terminal_row);
			checkScroll();
			break;
	}

	// Move the cursor with the output.
	ttySetCursor(getCharOffset(terminal_column, terminal_row));
}

// Put a string to the screen.
int terminalPutS(const char *str) {
	const char *ss;
	for (ss = str; *ss != '\0'; ss++)
		terminalPutC(*ss);
	return ss-str;
}

// Set the cursor to the position given by offset number of characters
void ttySetCursor(unsigned short offset) {
	// Write the high byte of the offset
	outb(TTY_COMMAND_PORT, TTY_HIGH_BYTE_CMD);
	outb(TTY_DATA_PORT, (unsigned char) ((offset >> 8) & 0xff));
	// Write the low byte of the offset
	outb(TTY_COMMAND_PORT, TTY_LOW_BYTE_CMD);
	outb(TTY_DATA_PORT, (unsigned char) offset & 0x00ff);
}

// Gets the current position of the cursor in the terminal
unsigned short ttyGetCursor() {
	unsigned short offset = 0;	// Initialise to 0
	// Write the high byte of the offset
	outb(TTY_COMMAND_PORT, TTY_HIGH_BYTE_CMD);
	offset = inb(TTY_DATA_PORT) << 8;
	// Write the low byte of the offset
	outb(TTY_COMMAND_PORT, TTY_LOW_BYTE_CMD);
	offset += inb(TTY_DATA_PORT);

	return offset;
}
