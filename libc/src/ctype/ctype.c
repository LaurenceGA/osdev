#include "ctype.h"

// Find the lower case equivalent of a character.
int tolower(int c) {
	if (isupper(c))
		return c + 0x20;
	return c;
}

// Find the upper case equivalent of a character.
int toupper(int c) {
	if (islower(c))
		return c - 0x20;
	return c;
}

// Check to see if the character is either a digit or a letter.
int isalnum(int c) {
	return isalpha(c) || isdigit(c);
}

// Check if a character is a letter.
int isalpha(int c) {
	return isupper(c) || islower(c);
}

// Check if a character is a standard blank character.
int isblank(int c) {
	return c == ' ' || c == '\t';
}

// Check if a character is a standard whitespace character.
int isspace(int c) {
	return c == '\n' || isblank(c) || c == '\r' || c == '\v' || c == '\f';
}

// Check if a character is an ascii digit '0' - '9'.
int isdigit(int c) {
	return c > 0x2f && c < 0x40;
}

// Check if c is a lowercase letter.
int islower(int c) {
	return  c > 0x60 && c < 0x7b;
}

// Check if c is an uppercase letter.
int isupper(int c) {
	return c > 0x40 && c < 0x5b;
}

// Check if the character is a control character.
int iscntrl(int c) {
	return c < 0x20 || c == 0x7f;
}

// Check if a character is printable.
int isprint(int c) {
	return c > 0x1f && c < 0x7f;
}

// Check if a character has a graphical representation.
int isgraph(int c) {
	return c > 0x20 &&  c < 0x7e;
}

// Check if a character is a punctuation character.
int ispunct(int c) {
	return (c > 0x20 && c < 0x30)  || (c > 0x39 && c < 0x41) ||
		(c > 0x5a && c < 0x61) || (c > 0x7c && c < 0x7f);
}

// Check if the character is a character that could be found in a hexadecimal
// number.
int isxdigit(int c) {
	return isdigit(c) || (c > 0x40 && c < 0x47) || (c > 0x60 && c < 0x68);
}
