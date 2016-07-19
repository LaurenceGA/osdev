#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "interrupts.h"
#include "stdbool.h"

// KEY CODES - fixed length of 1 byte
// bits 7-5 (range 8) = row
// bits 4-0 (range 32) = col
enum KEY_CODE {
/*R0*/	KEY_ESC, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8,
	KEY_F9, KEY_F10, KEY_F11, KEY_F12, KEY_PRSCR, KEY_INS, KEY_DEL,
/*R1*/	KEY_GRAVE=0x20, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8,
	KEY_9, KEY_0, KEY_MINUS, KEY_EQU, KEY_BCKSPC,
/*R2*/	KEY_TAB=0x40, KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T, KEY_Y, KEY_U, KEY_I,
	KEY_O, KEY_P, KEY_LBRACKET, KEY_RBRACKET, KEY_BSLASH,
/*R3*/	KEY_CAPS=0x50, KEY_A, KEY_S, KEY_D, KEY_F, KEY_G, KEY_H, KEY_J, KEY_K,
	KEY_L, KEY_SMICOLON, KEY_QUOTE, KEY_ENTER,
/*R4*/	KEY_LSHIFT=0x80, KEY_Z, KEY_X, KEY_C, KEY_V, KEY_B, KEY_N, KEY_M,
	KEY_COMMA, KEY_FSTOP, KEY_FSLASH, KEY_RSHIFT,
/*R5*/	KEY_LCTRL=0x90, KEY_SUPER, KEY_LALT, KEY_SPACE, KEY_RALT, KEY_RCTRL,
	KEY_MENU, KEY_LEFT, KEY_HOME, KEY_UP, KEY_PGUP, KEY_DOWN, KEY_PGDOWN,
	KEY_RIGHT, KEY_END
};


void initKBD();
bool isDown(enum KEY_CODE code);
void KBDinterrupt(struct cpu_state cpu, unsigned int interrupt, struct stack_state stack);

#endif // KEYBOARD_H
