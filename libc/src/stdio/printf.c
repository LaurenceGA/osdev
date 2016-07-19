#include <stdarg.h>
#include "stdbool.h"

#include "string.h"
#include "stdio.h"
#include "ctype.h"
#include "stdlib.h"

#include "tty.h"

// %[flags][width][.precision][length]specifier
struct formatSpecifier {
	char flags[8];
	int width;
	int precision;
	// length
	char specifier;
};

// Performs a linear search to see if target belongs to an array of chars
// group should be null terminated
static bool inGroup(char target, const char *group) {
	while (*group) {
		if (target == *group++)
			return true;
	}
	return false;
}

// prases a format specifier and it's extra
static int parseSpecifier(const char *sp, struct formatSpecifier *specifier) {
	char flags[] = {'0', '#', '+', '-', 0};
	int length = 0;
	// Parse flags
	char *fp = specifier->flags;
	while (inGroup(*sp, flags)) {
		*fp++ = *sp++;
		length++;
	}
	*fp = 0; // Null terminate
	// Parse width
	for (specifier->width = 0; isdigit(*sp); sp++) {
		specifier->width = 10 * specifier->width + (*sp - '0');
		length++;
	}
	// Parse precision
	if (*sp == '.') {
		sp++;
		length++;
		for (specifier->precision = 0; isdigit(*sp); sp++) {
			specifier->precision = 10 * specifier->precision + (*sp - '0');
			length++;
		}
	}
	// Parse specifier
	// The next character must be the specifier
	specifier->specifier = *sp;
	length++;
	return length;
}

// Insert num chars (c) into a string from start
static void insertChars(char *start, char c, int num) {
	memmove(start + num, start, strlen(start)+1);
	memset(start, c, num);
}

/**
 * Print a formatted string to console. Possible formatters:
 * c 	-- Print character
 * x 	-- Convert integer value to hexadecimal string
 * s 	-- Print a null-terminated string
 * d/i	-- integer
 */
int printf(const char *format, ...) {
	va_list parameters;
	va_start(parameters, format);
	int amount = 0;

	while (*format != '\0') {
		if (*format == '%') {
			format++;

			char buffer[2048] = {0};
			// char *buffer = (char *) malloc(sizeof(char) * 1024);

			struct formatSpecifier spec = {{0}, 0, 0, 0};
			format += parseSpecifier(format, &spec);

			switch(spec.specifier) {
			case '%':
				buffer[0] = '%';
				buffer[1] = '\0';
			break;

			case 'c': {
				char c = (char) va_arg(parameters, int);
				buffer[0] = c;
				buffer[1] = '\0';
			}
			break;

			case 'x': {
				char *buff = buffer;
				if (inGroup('#', spec.flags)) {
					strcpy(buff, "0x");
					buff += 2;
				}
				int i = (int) va_arg(parameters, int);
				itoa(i, buff, 16);
				// If left justified, no '0' padding
				if (!inGroup('-', spec.flags) && inGroup('0', spec.flags)) {
					int pad = spec.width - strlen(buffer);
					if (pad >= 0) {
						insertChars(buff, '0', pad);
					}
				}
			}
			break;

			case 's': {
				char *s = va_arg(parameters, char*);
				strcpy(buffer, s);
			}
			break;

			case 'i':	// fallthrough
			case 'd': {
				char *buff = buffer;
				int i = (int) va_arg(parameters, int);
				if (inGroup('+', spec.flags) && i >= 0) {
					*buff = '+';
					buff++;
					*buff = '\0';
				}
				itoa(i, buff, 10);
				if (i < 0)
					buff++;
				// If left justified, no '0' padding
				if (!inGroup('-', spec.flags) && inGroup('0', spec.flags)) {
					int pad = spec.width - strlen(buffer);
					if (pad >= 0) {
						insertChars(buff, '0', pad);
					}
				}
			}
			break;

			default:
				buffer[0] = '%';
				buffer[1] = spec.specifier;
				buffer[2] = '\0';
			break;
			}

			// We have our buffer string mostly done
			int len = strlen(buffer);
			int pad = spec.width - len;
			if (pad >= 0) {
				if (inGroup('-', spec.flags)) {
					memset(buffer+len, ' ', pad);
					buffer[len+pad] = '\0';
				} else
					insertChars(buffer, ' ', pad);
			}
			amount += terminalPutS(buffer);
			// free(buffer);
		} else {
			putchar(*format++);
			amount++;
		}
	}
	va_end(parameters);
	return amount;
}
