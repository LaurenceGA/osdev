;
; A simple boot sector program that says hello and then goodbye
;
[org 0x7c00]
	mov bx, hello_message	; Print hello
	call print_string

	mov ax, 0x0af2
	call print_hex

	mov bx, goodbye_message	; Print goodbye
	call print_string

	jmp $	; Infinite loop

%include "print_string.asm"	; Make sure we can print strings
%include "print_hex.asm"

; data
CR equ 0xd	; '\r'
LF equ 0xa	; '\n'
hello_message:
	db 'Hi there!', CR, LF, 0
goodbye_message:
	db 'Bye!', CR, LF, 0

times 510-($-$$) db 0	; pad program out to 510th byte

dw 0xaa55	; Magic number tells the boot loader that this is a boot sector
