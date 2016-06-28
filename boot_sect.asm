;
; A simple boot sector program that says hello and then goodbye
;
[org 0x7c00]
	mov bp, 0x8000		; Move the stack out of the way
	mov sp, bp

	mov bx, hello_message	; Print hello
	call print_string

	mov [BOOT_DRIVE], dl

	mov bx, 0x9000		; 5 secotrs 0x0000(ES):0x9000(BX)
	mov dh, 5
	mov dl, [BOOT_DRIVE]
	call disk_load



	mov ax, [0x9000]	; Print first laoded word
	call print_hex

	mov ax, [0x9000+512]	; Print first laoded word
	call print_hex

	mov bx, goodbye_message	; Print goodbye
	call print_string

	jmp $	; Infinite loop

%include "print_string.asm"	; Make sure we can print strings
%include "print_hex.asm"
%include "disk_load.asm"

; data
BOOT_DRIVE: db 0

CR: equ 0xd	; '\r'
LF: equ 0xa	; '\n'
hello_message:
	db 'Hi there!', CR, LF, 'Loading from disk...', CR, LF, 0
goodbye_message:
	db 'Bye!', CR, LF, 0

times 510-($-$$) db 0	; pad program out to 510th byte
dw 0xaa55	; Magic number tells the boot loader that this is a boot sector

; Make up some data
times 256 dw 0xdada
times 256 dw 0xface
times 2048 dw 0
