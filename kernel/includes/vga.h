#ifndef _VGA_H
#define _VGA_H

#include <stdint.h>

static uint16_t *const VIDEO_MEMORY = (uint16_t *) 0xB8000;
static const size_t VGA_HEIGHT = 25;
static const size_t VGA_WIDTH  = 80;

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

#endif // _VGA_H

