global _start
_start:
	; mov bx, 0x7c0
	; mov ds, bx

	mov ah, 0x0e
	mov al, 'X'
	int 0x10

	mov bx, hello_message	; Print hello
	call print_string

	jmp $

extern print_string
CR: equ 0xd	; '\r'
LF: equ 0xa	; '\n'
hello_message:
	db 'Hi there!', CR, LF, 0

times 510-($-$$) db 0	; pad program out to 510th byte
dw 0xaa55		; Magic number tells the boot loader that this is a boot sector
times 512*4 dw 0
