; these are functions dealing with the gdt and idt
; see also descriptor_tables.[c,h]

global GDTLoadSet

SEGSEL_KERNEL_CS equ 0x08
SEGSEL_KERNEL_DS equ 0x10

section .text

; load the gdt into the cpu, and enter the kernel segments
GDTLoadSet:
	mov eax, [esp+4]	; fetch gdt_ptr from parameter stack
	lgdt [eax]		; load gdt table

	; load cs segment by doing a far jump
	jmp SEGSEL_KERNEL_CS:.reloadSegments

.reloadSegments:
	; we only use one segment for data
	mov ax, SEGSEL_KERNEL_DS
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov gs, ax
	mov fs, ax
	ret
