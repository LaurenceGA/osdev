#ifndef _TTY_H
#define _TTY_H

#include <stddef.h>
#include <stdint.h>

// Set some initial values of the screen.
void initTerminal();

// Set the output colour of characters and their background.
void terminalSetColour(uint8_t);
// Move to a specific row and column to place a character on screen.
void terminalMvPutC(char, uint8_t, int, int);
// Put a character to the screen.
void terminalPutC(char);
// Put a string to the screen.
void terminalPutS(const char *);

#endif // _TTY_H
