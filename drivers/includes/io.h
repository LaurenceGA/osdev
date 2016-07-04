#ifndef _IO_H
#define _IO_H

/* outb
 * Sends the data to the given I/O port
 * implemented in io.asm
 *
 * port is the I/O port address the data is sent to
 * data is the actual information to send to the port
 */
void outb(unsigned short port, unsigned char data);

/* inb
 * Reads data from the given I/O port
 * implemented in io.asm
 *
 * port is the I/O port address the data is read from
 */
unsigned char inb(unsigned short port);

#endif
