#ifndef TTY_H
#define TTY_H



// Moves the tty cursor by number of characters offset
void ttySetCursor(unsigned short offset);
// Gets the number of charachters the cursor is current offset by
unsigned short ttyGetCursor();

#endif
