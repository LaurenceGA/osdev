#include "include/io.h"

// I/O ports
unsigned short TTY_COMMAND_PORT = 0x3d4;
unsigned short TTY_DATA_PORT = 0x3d5;

// I/O port commands
unsigned char TTY_HIGH_BYTE_CMD = 14;
unsigned char TTY_LOW_BYTE_CMD = 15;

// Moves the cursor to the position given by videoMemory + offset
void ttyMoveCursor(unsigned short offset) {
	offset /= 2;	// The port uses character offset, so we must convert
	// Write the high byte of the offset
	outb(TTY_COMMAND_PORT, TTY_HIGH_BYTE_CMD);
	outb(TTY_DATA_PORT, (unsigned char) (offset & 0xff00));
	// Write the low byte of the offset
	outb(TTY_COMMAND_PORT, TTY_LOW_BYTE_CMD);
	outb(TTY_DATA_PORT, (unsigned char) (offset & 0x00ff));
}
