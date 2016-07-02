#ifndef _TTY_H
#define _TTY_H

#include <stddef.h>
#include <stdint.h>

extern void initTerminal();

extern void terminalSetColour(uint8_t);
extern void terminalMvPutC(char, uint8_t, size_t, size_t);
extern void terminalPutC(char);
extern void terminalPutS(const char *);

#endif // _TTY_H

