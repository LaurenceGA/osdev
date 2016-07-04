#ifndef _TTY_H
#define _TTY_H

#include <stddef.h>
#include <stdint.h>

extern void initTerminal();

extern void terminalSetColour(uint8_t);
extern void terminalMvPutC(char, uint8_t, int, int);
extern void terminalPutC(char);
extern void terminalPutS(const char *);

#endif // _TTY_H

