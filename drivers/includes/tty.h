#ifndef _TTY_H
#define _TTY_H

#include "stdint.h"

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

// Set some initial values of the screen.
void initTerminal();

// Set the output colour of characters and their background.
void terminalSetColour(uint8_t);
// Move to a specific row and column to place a character on screen.
void terminalMvPutC(char, uint8_t, int, int);
// Put a character to the screen.
void terminalPutC(char);
// Put a string to the screen.
int terminalPutS(const char *);

// Moves the tty cursor by number of characters offset
void ttySetCursor(unsigned short offset);
// Gets the number of charachters the cursor is current offset by
unsigned short ttyGetCursor();

// Clears the terminal
void clearTerminal();
// Turns terminal coordinates into a character offset
unsigned int getCharOffset(unsigned int x, unsigned int y);

// Check if the terminal needs to be scrolled.
void checkScroll();
// Shuffle each terminal row down one, and blank the last row.
void doScroll();

#endif // _TTY_H
