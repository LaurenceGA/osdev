#include "keyboard.h"
#include "interrupts.h"
#include "stdio.h"
#include "io.h"
#include "pic.h"
#include "stdlib.h"
#include <stdbool.h>

#define KBD_DATA_PORT 0x60
#define KBD_CMD_PORT 0x64

struct keyData {
	bool isPressed;
	char ASCII;
	char altASCII;
};

static bool capsLock = false;

// 1 byte index = 256 entries
static struct keyData keys[256];

// 9 bits, 512 entries
static enum KEY_CODE scanToKeycode[512];

// Where size is max scan codes
static unsigned char scanCodeBuffer[6];
static unsigned char *SCBufferHead = (unsigned char*) &scanCodeBuffer;

#define BUFFER_CAPACITY 32
struct eventQueue {
	unsigned int contents[BUFFER_CAPACITY];
	int count;
	unsigned int *front;
} keyEventBuffer;

void enqueue(struct eventQueue *queue, unsigned int data) {
	int backIndex = ((queue->front - queue->contents) + queue->count++) % BUFFER_CAPACITY;
	queue->contents[backIndex] = data;
}

unsigned int dequeue(struct eventQueue *queue) {
	if (queue->count == 0)
		return 0;
	queue->count--;
	int currentIndex = ((queue->front - queue->contents)) % BUFFER_CAPACITY;
	int newFrontIndex = (currentIndex + 1) % BUFFER_CAPACITY;
	// printf("\n%d\n", newFrontIndex);
	queue->front = &queue->contents[newFrontIndex];
	return queue->contents[currentIndex];
}

bool isEmpty(struct eventQueue *queue) {
	return queue->count == 0;
}

static void registerKey(enum KEY_CODE keyCode, unsigned short scanCode,
			char ASCII, char altASCII) {
	scanToKeycode[scanCode] = keyCode;
	keys[keyCode].isPressed = false;
	keys[keyCode].ASCII = ASCII;
	keys[keyCode].altASCII = altASCII;
}

// Returns true if finished scan code aquisition
static bool readScanCode() {
	unsigned char codeByte = inb(KBD_DATA_PORT);
	*SCBufferHead++ = codeByte;
	if (codeByte != 0xe0)	// More to come?
		return true;
	return false;;
}

static char getASCII(enum KEY_CODE code) {
	char ch = 0;
	if (isDown(KEY_LSHIFT) || isDown(KEY_RSHIFT)) {
		ch = keys[code].altASCII;
		if (capsLock) {
			if (ch >= 'A' && ch <= 'Z') {
				ch = keys[code].ASCII;;
			}
		}
	} else {
		ch = keys[code].ASCII;
		if (capsLock) {
			if (ch >= 'a' && ch <= 'z') {
				ch = keys[code].altASCII;;
			}
		}
	}
	return ch;
}

void printKeyEvent(unsigned int keyPacket) {
	unsigned char ASCII = (unsigned char) ((keyPacket & 0xff000000) >> 24);
	// printf("%0#10x : %0#4x\n", keyPacket, ASCII);
	if (keyPacket & 1)
		printf("Release ");
	else
		printf("Pressed ");
	printf("Key %#x ", (keyPacket & 0x00ff0000) >> 16);
	printf("ASCII: ");
	if (ASCII)
		printf("'%c'", ASCII);
	else
		printf("None.");
	if (keyPacket & 2) printf(" shft");
	if (keyPacket & 4) printf(" ctrl");
	if (keyPacket & 8) printf(" caps");
	if (keyPacket & 16) printf(" repeat");
	printf("\n");
}

unsigned int createKeyEvent(unsigned short scanCode) {
	/*
	Key event packet
	32 bits
	bit	| description
	----------------------
	31-23	| ASCII
	22-14	| Key code
	4	| 1 = Repeat
	3	| Caps lock on
	2	| 1 = ctrl down
	1	| 1 = shift down
	0	| 0 = press, 1 = release
	*/
	unsigned int keyEvent = 0;
	enum KEY_CODE keycode;
	if (scanCode & 0x80) {
		// Set key event flag for released
		keyEvent |= 0x1;
		// Remove break bit flag
		keycode = scanToKeycode[scanCode & 0xff7f];
		// Set keycode bits 22-14
		keyEvent |= keycode << 16;
		keys[keycode].isPressed = false;
		if (keycode == KEY_CAPS) {
			capsLock = !capsLock;
		}
	} else {
		keycode = scanToKeycode[scanCode];
		keyEvent |= keycode << 16;
		keyEvent |= (keys[keycode].isPressed ? 1 : 0) << 4;
		keys[keycode].isPressed = true;
	}
	if (keycode == 0) {
		printf("0 keycode: %#x scancode\n", scanCode);
	}
	keyEvent |= getASCII(keycode) << 24;
	keyEvent |= (capsLock ? 1 : 0) << 3;
	keyEvent |= (keys[KEY_LCTRL].isPressed || keys[KEY_RCTRL].isPressed) << 2;
	keyEvent |= (keys[KEY_LSHIFT].isPressed || keys[KEY_RSHIFT].isPressed) << 1;

	return keyEvent;
}

char getchar() {
	unsigned int event = 0;
	while (true) {
		if ((event = dequeue(&keyEventBuffer)) == 0) {
			continue;
		}
		unsigned char ASCII = (unsigned char) ((event & 0xff000000) >> 24);
		if (event && (event & 1) == 0) {
			return ASCII;
		}
	}
}

void KBDinterrupt(struct cpu_state cpu, unsigned int interrupt,
			struct stack_state stack) {
	if (readScanCode()) {
		// Scan code is read each time, this is only entered
		// if the whole scan code is in the buffer
		unsigned short code = 0;
		for (unsigned char *bp = scanCodeBuffer; bp < SCBufferHead; bp++) {
			code = (code << 8) + *bp;
		}

		// printKeyEvent(createKeyEvent(code));
		enqueue(&keyEventBuffer, createKeyEvent(code));
		// printKeyEvent(dequeue(&keyEventBuffer));

		// enum KEY_CODE keycode;
		/*if (code & 0x80) {
			// Remove break bit flag
			keycode = scanToKeycode[code & 0xff7f];
			keys[keycode].isPressed = false;
			if (keycode == KEY_CAPS) {
				capsLock = !capsLock;
			}
		} else {
			keycode = scanToKeycode[code];
			keys[keycode].isPressed = true;
			char ASCII = getASCII(keycode);
			if (ASCII) {
				// prin/tf("%c", ASCII);
			}
		}*/

		SCBufferHead = scanCodeBuffer; // Reset to start of buffer
	}

	// Arguments unused - this supresses warning
	(void) interrupt;
	(void) cpu;
	(void) stack;
}


bool isDown(enum KEY_CODE code) {
	return keys[code].isPressed;
}

void initKBD() {
	registerInterruptHandler(KBD_INT_IDX, KBDinterrupt);

	// key even buffer initialisation
	keyEventBuffer.count = 0;
	keyEventBuffer.front = keyEventBuffer.contents;

	// R0
	registerKey(KEY_ESC, 0x1, 0, 0);
	registerKey(KEY_F1, 0x3b, 0, 0);
	registerKey(KEY_F2, 0x3c, 0, 0);
	registerKey(KEY_F3, 0x3d, 0, 0);
	registerKey(KEY_F4, 0x3e, 0, 0);
	registerKey(KEY_F5, 0x3f, 0, 0);
	registerKey(KEY_F6, 0x40, 0, 0);
	registerKey(KEY_F7, 0x41, 0, 0);
	registerKey(KEY_F8, 0x42, 0, 0);
	registerKey(KEY_F9, 0x43, 0, 0);
	registerKey(KEY_F10, 0x44, 0, 0);
	registerKey(KEY_F11, 0x57, 0, 0);
	registerKey(KEY_F12, 0x58, 0, 0);
	registerKey(KEY_PRSCR, 0xe037, 0, 0);
	registerKey(KEY_INS, 0xe052, 0, 0);
	registerKey(KEY_DEL, 0xe053, 0, 0);

	// R1
	registerKey(KEY_GRAVE, 0x29, '`', '~');
	registerKey(KEY_1, 0x2, '1', '!');
	registerKey(KEY_2, 0x3, '2', '@');
	registerKey(KEY_3, 0x4, '3', '#');
	registerKey(KEY_4, 0x5, '4', '$');
	registerKey(KEY_5, 0x6, '5', '%');
	registerKey(KEY_6, 0x7, '6', '^');
	registerKey(KEY_7, 0x8, '7', '&');
	registerKey(KEY_8, 0x9, '8', '*');
	registerKey(KEY_9, 0xa, '9', '(');
	registerKey(KEY_0, 0xb, '0', ')');
	registerKey(KEY_MINUS, 0xc, '-', '_');
	registerKey(KEY_EQU, 0xd, '=', '+');
	registerKey(KEY_BCKSPC, 0xe, '\b', 0);

	// R2
	registerKey(KEY_TAB, 0xf, '\t', 0);
	registerKey(KEY_Q, 0x10, 'q', 'Q');
	registerKey(KEY_W, 0x11, 'w', 'W');
	registerKey(KEY_E, 0x12, 'e', 'E');
	registerKey(KEY_R, 0x13, 'r', 'R');
	registerKey(KEY_T, 0x14, 't', 'T');
	registerKey(KEY_Y, 0x15, 'y', 'Y');
	registerKey(KEY_U, 0x16, 'u', 'U');
	registerKey(KEY_I, 0x17, 'i', 'I');
	registerKey(KEY_O, 0x18, 'o', 'O');
	registerKey(KEY_P, 0x19, 'p', 'P');
	registerKey(KEY_LBRACKET, 0x1a, '[', '{');
	registerKey(KEY_RBRACKET, 0x1b, ']', '}');
	registerKey(KEY_BSLASH, 0x2b, '\\', '|');

	// R3
	registerKey(KEY_CAPS, 0x3a, 0, 0);
	registerKey(KEY_A, 0x1e, 'a', 'A');
	registerKey(KEY_S, 0x1f, 's', 'S');
	registerKey(KEY_D, 0x20, 'd', 'D');
	registerKey(KEY_F, 0x21, 'f', 'F');
	registerKey(KEY_G, 0x22, 'g', 'G');
	registerKey(KEY_H, 0x23, 'h', 'H');
	registerKey(KEY_J, 0x24, 'j', 'J');
	registerKey(KEY_K, 0x25, 'k', 'K');
	registerKey(KEY_L, 0x26, 'l', 'L');
	registerKey(KEY_SMICOLON, 0x27, ';', ':');
	registerKey(KEY_BSLASH, 0x28, '\'', '"');
	registerKey(KEY_ENTER, 0x1c, '\n', 0);

	// R4
	registerKey(KEY_LSHIFT, 0x2a, 0, 0);
	registerKey(KEY_Z, 0x2c, 'z', 'Z');
	registerKey(KEY_X, 0x2d, 'x', 'X');
	registerKey(KEY_C, 0x2e, 'c', 'C');
	registerKey(KEY_V, 0x2f, 'v', 'V');
	registerKey(KEY_B, 0x30, 'b', 'B');
	registerKey(KEY_N, 0x31, 'n', 'N');
	registerKey(KEY_M, 0x32, 'm', 'M');
	registerKey(KEY_COMMA, 0x33, ',', '<');
	registerKey(KEY_FSTOP, 0x34, '.', '>');
	registerKey(KEY_FSLASH, 0x35, '/', '?');
	registerKey(KEY_RSHIFT, 0x36, 0, 0);

	// R5
	registerKey(KEY_LCTRL, 0x1d, 0, 0);
	registerKey(KEY_SUPER, 0xe05b, 0, 0);
	registerKey(KEY_LALT, 0x38, 0, 0);
	registerKey(KEY_SPACE, 0x39, ' ', 0);
	registerKey(KEY_RALT, 0xe038, 0, 0);
	registerKey(KEY_RCTRL, 0xe01d, 0, 0);
	registerKey(KEY_MENU, 0xe05d, 0, 0);
	registerKey(KEY_LEFT, 0xe04b, 0, 0);
	registerKey(KEY_HOME, 0xe047, 0, 0);
	registerKey(KEY_UP, 0xe048, 0, 0);
	registerKey(KEY_PGUP, 0xe049, 0, 0);
	registerKey(KEY_DOWN, 0xe050, 0, 0);
	registerKey(KEY_PGDOWN, 0xe051, 0, 0);
	registerKey(KEY_RIGHT, 0xe04d, 0, 0);
	registerKey(KEY_END, 0xe04f, 0, 0);
}
