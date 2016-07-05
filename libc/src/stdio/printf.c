#include <stdarg.h>
#include <stdbool.h>

#include "string.h"
#include "stdio.h"
#include "ctype.h"

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
			struct formatSpecifier spec = {{0}, 0, 0, 0};
			format += parseSpecifier(format, &spec);
			switch(spec.specifier) {
			case '%':
				putchar('%');
				amount++;
			break;

			case 'c': {
				char c = (char) va_arg(parameters, int);
				putchar(c);
				amount++;
			}
			break;

			case 'x': {
				// int i = (int) va_arg(parameters, int);
				// amount += printf("%x", i);
			}
			break;

			case 's': {
				char *s = va_arg(parameters, char*);
				terminalPutS(s);
				amount += strlen(s);
			}
			break;

			case 'i':	// fallthrough
			case 'd': {
				// int i = (int) va_arg(parameters, int);
				// amount += printf("%i", i);
			}
			break;

			default:
				putchar('%');
				putchar(spec.specifier);
				amount += 2;
			break;
			}
		} else {
			putchar(*format++);
			amount++;
		}
	}
	va_end(parameters);
	return amount;
}
