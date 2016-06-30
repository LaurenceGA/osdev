gdt_start:

gdt_null:	; Mandatory null descriptor
	; times 2 dd 0	; 2 double words
	dd 0x0
	dd 0x0

gdt_code:	; Code segment descriptor
; base=0x0, limit=0xfffff
; 1st flags:	(present)1 (privilege)00 (desciptor type)1 -> 1001b
; type flags:	(code)1 (conforming)0 (readable)1 (accessed)0 -> 1010b
; 2nd flags:	(granularity)1 (32-bit default)1 (64-bit seg)0 (AVL)0 -> 1100b
	dw 0xffff	; Limit bits 0-15
	dw 0x0		; Base bits 0-15
	db 0x0		; Base bits 16-23
	db 10011010b	; 1st flags, type flags
	db 11001111b	; 2nd flags, Limit bits 16-19
	db 0x0		; Base bits 24-31

gdt_data:	; Data segment descriptor
; Same as code except type flags
; type flags:	(code)0 (expand down)0 (writable)1 (accessed)0 -> 0010b
	dw 0xffff	; Limit bits 0-15
	dw 0x0		; Base bits 0-15
	db 0x0		; Base bits 16-23
	db 10010010b	; 1st flags, type flags
	db 11001111b	; 2nd flags, Limit bits 16-19
	db 0x0		; Base bits 24-31
gdt_end:	; So we can calculate the size

; GDT descriptor
gdt_descriptor:
	dw gdt_end - gdt_start - 1	; One less than true size
	dd gdt_start

; Handy constants for the offsets (to the gdt table) our segment
; registers should use
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
