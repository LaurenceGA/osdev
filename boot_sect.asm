;
; A simple boot sector program that says hello and then goodbye
;
[org 0x7c00]
; global _start
; _start:
	mov bp, 0x9000		; Move the stack out of the way
	mov sp, bp

	; mov bx, 0x7c0
	; mov cs, bx
	; mov ds, bx

	mov bx, hello_message	; Print hello
	call print_string

	; mov [BOOT_DRIVE], dl

	; mov bx, 0x9000		; 5 secotrs 0x0000(ES):0x9000(BX)
	; mov dh, 5
	; mov dl, [BOOT_DRIVE]
	; call disk_load

	; mov ax, [0x9000]	; Print first laoded word
	; call print_hex

	; mov ax, [0x9000+512]	; Print first laoded word
	; call print_hex

	call switch_to_pm

	jmp $	; Infinite loop

%include "print_string.asm"	; Make sure we can print strings
; extern print_string
%include "print_hex.asm"
%include "disk_load.asm"
%include "pm/gdt.asm"
%include "pm/switch_to_pm.asm"
%include "pm/print_string_pm.asm"

[bits 32]
; Where we end up after calling the switch
BEGIN_PM:
	mov ebx, goodbye_message
	call print_string_pm

	jmp $	; Hang

; data
BOOT_DRIVE: db 0

CR: equ 0xd	; '\r'
LF: equ 0xa	; '\n'
hello_message:
	db 'Hi there!', CR, LF, 'Switching to protected mode...', CR, LF, 0
goodbye_message:
	db 'In protected mode!!', 0

times 510-($-$$) db 0	; pad program out to 510th byte
dw 0xaa55		; Magic number tells the boot loader that this is a boot sector

; Make up some data
times 256 dw 0xdada	; 1 sector
times 256 dw 0xface
times 512*4 dw 0	; Chuck in 4 more sectors
