#include "include/tty.h"
#include "include/io.h"

// I/O ports
unsigned short TTY_COMMAND_PORT = 0x3d4;
unsigned short TTY_DATA_PORT = 0x3d5;

// I/O port commands
unsigned char TTY_HIGH_BYTE_CMD = 14;
unsigned char TTY_LOW_BYTE_CMD = 15;

// Set the cursor to the position given by offset number of characters
void ttySetCursor(unsigned short offset) {
	// Write the high byte of the offset
	outb(TTY_COMMAND_PORT, TTY_HIGH_BYTE_CMD);
	outb(TTY_DATA_PORT, (unsigned char) (offset & 0xff00));
	// Write the low byte of the offset
	outb(TTY_COMMAND_PORT, TTY_LOW_BYTE_CMD);
	outb(TTY_DATA_PORT, (unsigned char) (offset & 0x00ff));
}

// Gets the current position of the cursor in the terminal
unsigned short ttyGetCurosr() {
	unsigned short offset = 0;	// Initialise to 0
	// Write the high byte of the offset
	outb(TTY_COMMAND_PORT, TTY_HIGH_BYTE_CMD);
	offset = inb(TTY_DATA_PORT) << 8;
	// Write the low byte of the offset
	offset += inb(TTY_DATA_PORT);
	outb(TTY_COMMAND_PORT, TTY_LOW_BYTE_CMD);

	return offset;
}
