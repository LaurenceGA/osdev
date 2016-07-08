#include "keyboard.h"
#include "interrupts.h"
#include "stdio.h"
#include "io.h"
#include "pic.h"
#include "stdlib.h"
#include <stdbool.h>

#define KBD_DATA_PORT 0x60

/* Alphabet */
#define KBD_SC_A        0x1e
#define KBD_SC_B        0x30
#define KBD_SC_C        0x2e
#define KBD_SC_D        0x20
#define KBD_SC_E        0x12
#define KBD_SC_F        0x21
#define KBD_SC_G        0x22
#define KBD_SC_H        0x23
#define KBD_SC_I        0x17
#define KBD_SC_J        0x24
#define KBD_SC_K        0x25
#define KBD_SC_L        0x26
#define KBD_SC_M        0x32
#define KBD_SC_N        0x31
#define KBD_SC_O        0x18
#define KBD_SC_P        0x19
#define KBD_SC_Q        0x10
#define KBD_SC_R        0x13
#define KBD_SC_S        0x1f
#define KBD_SC_T        0x14
#define KBD_SC_U        0x16
#define KBD_SC_V        0x2f
#define KBD_SC_W        0x11
#define KBD_SC_X        0x2d
#define KBD_SC_Y        0x15
#define KBD_SC_Z        0x2c

/* Numeric keys */
#define KBD_SC_1        0x02
#define KBD_SC_2        0x03
#define KBD_SC_3        0x04
#define KBD_SC_4        0x05
#define KBD_SC_5        0x06
#define KBD_SC_6        0x07
#define KBD_SC_7        0x08
#define KBD_SC_8        0x09
#define KBD_SC_9        0x0a
#define KBD_SC_0        0x0b

/* Special keys */
#define KBD_SC_ENTER    0x1c
#define KBD_SC_SPACE    0x39
#define KBD_SC_BS       0x0e
#define KBD_SC_LSHIFT   0x2a
#define KBD_SC_RSHIFT   0x36
#define KBD_SC_DASH     0x0c
#define KBD_SC_EQUALS   0x0d
#define KBD_SC_LBRACKET 0x1a
#define KBD_SC_RBRACKET 0x1b
#define KBD_SC_BSLASH   0x2b
#define KBD_SC_SCOLON   0x27
#define KBD_SC_QUOTE    0x28
#define KBD_SC_COMMA    0x33
#define KBD_SC_DOT      0x34
#define KBD_SC_FSLASH   0x35
#define KBD_SC_TILDE    0x29
#define KBD_SC_CAPSLOCK 0x3a
#define KBD_SC_TAB      0x0f

static bool is_lshift_down = false;
static bool is_rshift_down = false;
static bool is_caps_lock_pressed = false;

unsigned char readScanCode() {
	return inb(KBD_DATA_PORT);
}

void KBDinterrupt(struct cpu_state cpu, unsigned int interrupt,
			struct stack_state stack) {
	unsigned char ASCIIchar = convertToASCII(readScanCode());
	if (ASCIIchar)
		putchar(ASCIIchar);
	// Arguments unused - this supresses warning
	(void) interrupt;
	(void) cpu;
	(void) stack;
}

static void toggle_caps_lock() {
	is_caps_lock_pressed = !is_caps_lock_pressed;
}

static unsigned char handle_caps_lock(unsigned char ch) {
	if (ch >= 'a' && ch <= 'z') {
		return ch + 'A' - 'a';
	}
	return ch;
}

static unsigned char handle_shift(unsigned char ch) {
	// Alphabetic characters
	if (ch >= 'a' && ch <= 'z') {
		return ch + 'A' - 'a';
	}

	// Number characters
	switch (ch) {
	case '0':
		return ')';
	case '1':
		return '!';
	case '2':
		return '@';
	case '3':
		return '#';
	case '4':
		return '$';
	case '5':
		return '%';
	case '6':
		return '^';
	case '7':
		return '&';
	case '8':
		return '*';
	case '9':
		return '(';
	default:
		break;
	}

	// Special charachters
	switch (ch) {
	case '-':
		return '_';
	case '=':
		return '+';
	case '[':
		return '{';
	case ']':
		return '}';
	case '\\':
		return '|';
	case ';':
		return ':';
	case '\'':
		return '\"';
	case ',':
		return '<';
	case '.':
		return '>';
	case '/':
		return '?';
	case '`':
		return '~';
	}

	return ch;
}

unsigned char convertToASCII(unsigned char scanCode) {
	unsigned char ch = 0;
	if (scanCode & 0x80) {	// Key released
		scanCode &= 0x7F; /* clear the bit set by key break */

		switch (scanCode) {
		case KBD_SC_LSHIFT:
			is_lshift_down = false;
			break;
		case KBD_SC_RSHIFT:
			is_rshift_down = false;
			break;
		case KBD_SC_CAPSLOCK:
			toggle_caps_lock();
			break;
		default:
			break;
		}

		return 0;
	}

	switch(scanCode) {
	case KBD_SC_A:
		ch = 'a';
		break;
	case KBD_SC_B:
		ch = 'b';
		break;
	case KBD_SC_C:
		ch = 'c';
		break;
	case KBD_SC_D:
		ch = 'd';
		break;
	case KBD_SC_E:
		ch = 'e';
		break;
	case KBD_SC_F:
		ch = 'f';
		break;
	case KBD_SC_G:
		ch = 'g';
		break;
	case KBD_SC_H:
		ch = 'h';
		break;
	case KBD_SC_I:
		ch = 'i';
		break;
	case KBD_SC_J:
		ch = 'j';
		break;
	case KBD_SC_K:
		ch = 'k';
		break;
	case KBD_SC_L:
		ch = 'l';
		break;
	case KBD_SC_M:
		ch = 'm';
		break;
	case KBD_SC_N:
		ch = 'n';
		break;
	case KBD_SC_O:
		ch = 'o';
		break;
	case KBD_SC_P:
		ch = 'p';
		break;
	case KBD_SC_Q:
		ch = 'q';
		break;
	case KBD_SC_R:
		ch = 'r';
		break;
	case KBD_SC_S:
		ch = 's';
		break;
	case KBD_SC_T:
		ch = 't';
		break;
	case KBD_SC_U:
		ch = 'u';
		break;
	case KBD_SC_V:
		ch = 'v';
		break;
	case KBD_SC_W:
		ch = 'w';
		break;
	case KBD_SC_X:
		ch = 'x';
		break;
	case KBD_SC_Y:
		ch = 'y';
		break;
	case KBD_SC_Z:
		ch = 'z';
		break;
	case KBD_SC_0:
		ch = '0';
		break;
	case KBD_SC_1:
		ch = '1';
		break;
	case KBD_SC_2:
		ch = '2';
		break;
	case KBD_SC_3:
		ch = '3';
		break;
	case KBD_SC_4:
		ch = '4';
		break;
	case KBD_SC_5:
		ch = '5';
		break;
	case KBD_SC_6:
		ch = '6';
		break;
	case KBD_SC_7:
		ch = '7';
		break;
	case KBD_SC_8:
		ch = '8';
		break;
	case KBD_SC_9:
		ch = '9';
		break;
	case KBD_SC_ENTER:
		ch = '\n';
		break;
	case KBD_SC_SPACE:
		ch = ' ';
		break;
	case KBD_SC_BS:
		ch = '\b';
		break;
	case KBD_SC_DASH:
		ch = '-';
		break;
	case KBD_SC_EQUALS:
		ch = '=';
		break;
	case KBD_SC_LBRACKET:
		ch = '[';
		break;
	case KBD_SC_RBRACKET:
		ch = ']';
		break;
	case KBD_SC_BSLASH:
		ch = '\\';
		break;
	case KBD_SC_SCOLON:
		ch = ';';
		break;
	case KBD_SC_QUOTE:
		ch = '\'';
		break;
	case KBD_SC_COMMA:
		ch = ',';
		break;
	case KBD_SC_DOT:
		ch = '.';
		break;
	case KBD_SC_FSLASH:
		ch = '/';
		break;
	case KBD_SC_TILDE:
		ch = '`';
		break;
	case KBD_SC_TAB:
		ch = '\t';
		break;
	case KBD_SC_LSHIFT:
		is_lshift_down = true;
		break;
	case KBD_SC_RSHIFT:
		is_rshift_down = true;
		break;
	default:
		return 0;
		break;
	}

	if (is_caps_lock_pressed) {
		ch = handle_caps_lock(ch);
	}

	if (is_lshift_down || is_rshift_down) {
		ch = handle_shift(ch);
	}

	return ch;
}

void initKBD() {
	registerInterruptHandler(KBD_INT_IDX, KBDinterrupt);
}
