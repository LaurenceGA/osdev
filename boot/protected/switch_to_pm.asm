[bits 16]
; Switch to protected mode
switch_to_pm:
	cli	; Clear interrupts
	lgdt [gdt_descriptor]	; Load up the descriptor table

	mov eax, cr0	; Switch the bit on the control register on
	or eax, 0x1
	mov cr0, eax

	jmp CODE_SEG:init_pm	; Make a far jump to flush cpu cache

[bits 32]
; Initialise registers and stack for pm
init_pm:
	mov ax, DATA_SEG	; Point all segment registers to data selectors
	mov ds, ax		; Defined in GDT
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov ebp, 0x90000	; Update stack position to top of free space
	mov esp, ebp

	call loader		; Get to well known label
