#include <stdio.h>
#include <stdarg.h>
#include <string.h>

int prf(const char *format, ...) {
	va_list parameters;
	va_start(parameters, format);
	int amount = 0;

	while (*format != '\0') {
		if (*format == '%') {
			format++;
			switch(*format) {
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
				int i = (int) va_arg(parameters, int);
				amount += printf("%x", i);
			}
			break;

			case 's': {
				char *s = va_arg(parameters, char*);
				printf("%s", s);	// to avoid the puts newline
				amount += strlen(s);
			}
			break;

			case 'i':	// fallthrough
			case 'd': {
				int i = (int) va_arg(parameters, int);
				amount += printf("%i", i);
			}
			break;

			default:
				putchar('%');
				putchar(*format);
				amount += 2;
			break;
			}
			format++;
		} else {
			putchar(*format++);
			amount++;
		}
	}
	va_end(parameters);
	return amount;
}

int main() {
	int written = prf("%s a %i%% printf %f copy: %x\n", "this is", 90, 200);

	printf("wrote %d characters\n", written);

	return 0;
}
