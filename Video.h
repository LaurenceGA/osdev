#ifndef VIDEO_H
#define VIDEO_H

extern enum COLOUR;

extern size_t    terminal_row;
extern size_t    terminal_column;
extern uint8_t   terminal_colour;
extern uint16_t *terminal_buffer;

extern uint16_t make_vga_entry(char, uint8_t);

extern void init_terminal();
extern void terminal_setcolour(uint8_t);
extern void terminal_putc(char);
extern void terminal_mvputc(char, uint8_t, size_t, size_t);
extern void terminal_puts(const char *);
extern void terminal_mvputs(const char *, uint8_t, size_t, size_t);

extern uint8_t make_colour(enum COLOUR, enum COLOUR);

#endif // VIDEO_H
