#include <stdarg.h>
#include <stdbool.h>

#include "string.h"
#include "stdio.h"

static void print(const char *data, int length) {
	for (int i = 0; i < length; i++)
		putchar((int) ((const unsigned char *) data)[i]);
}

/**
 * Print a formatted string to console. Possible formatters:
 *
 * c -- Convert value to a single char.
 * x -- Convert integer value to hexadecimal string.
 * s -- Print a null-terminated string
 */
int printf(const char* restrict format, ...) {
	va_list parameters;
	va_start(parameters, format);

	int written = 0;
	int amount;
	bool rejected_bad_specifier = false;

	while (*format != '\0') {
		if (*format != '%') {
		print_c:
			amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;
			print(format, amount);
			format += amount;
			written += amount;
			continue;
		}

		const char* format_begun_at = format;

		if ( *(++format) == '%' )
			goto print_c;

		if ( rejected_bad_specifier ) {
		incomprehensible_conversion:
			rejected_bad_specifier = true;
			format = format_begun_at;
			goto print_c;
		}

		if (*format == 'c') {
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			print(&c, sizeof(c));
		} else if (*format == 's') {
			format++;
			const char* s = va_arg(parameters, const char*);
			print(s, strlen(s));
		} else if (*format == 'x') {
			// TODO: Convert int to string.
		} else {
			goto incomprehensible_conversion;
		}
	}

	va_end(parameters);

	return written;
}

