;
; A simple boot sector program that says hello and then goodbye
;

[org 0x7c00]
global _start
_start:
KERNEL_OFFSET equ 0x1000	; Where we will load the kernel
	mov [BOOT_DRIVE], dl	; The bootloader stores the boot drive
				; in dl, so we grab that

	mov bp, 0x9000		; Move the stack out of the way
	mov sp, bp

	mov bx, msg_real_mode	; Print welcoming message
	call print_string

	call load_kernel

	call switch_to_pm

	jmp $	; Infinite loop

%include "real/print_string.asm"	; Make sure we can print strings
%include "real/print_hex.asm"
%include "real/disk_load.asm"
%include "protected/gdt.asm"
%include "protected/switch_to_pm.asm"
%include "protected/print_string_pm.asm"

[bits 16]
load_kernel:
	mov bx, msg_loading_kernel	; Say what's happening
	call print_string

	mov bx, KERNEL_OFFSET	; Where we will load in the kernel
	mov dh, 15		; Number of sectors to load
	mov dl, [BOOT_DRIVE]	; Where to look for kernel
	call disk_load

	ret

[bits 32]
; Where we end up after calling the switch
BEGIN_PM:
	mov ebx, msg_protected_mode
	call print_string_pm

	call KERNEL_OFFSET

	jmp $	; Hang

; data
BOOT_DRIVE: db 0

CR: equ 0xd	; '\r'
LF: equ 0xa	; '\n'
msg_real_mode:
	db 'Started in 16-bit real mode', CR, LF, 0
msg_loading_kernel:
	db 'Loading kernel into RAM', CR, LF, 0
msg_protected_mode:
	db 'In 32-bit protected mode', 0

