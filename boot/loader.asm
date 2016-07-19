
MAGIC_NUMBER		equ 0x1BADB002
FLAGS				equ 0x0
CHECKSUM			equ -MAGIC_NUMBER
KERNEL_STACK_SIZE	equ 4096

section .text
align 4
dd MAGIC_NUMBER
dd FLAGS
dd CHECKSUM

section .bss
align 4
kernel_stack:
resb KERNEL_STACK_SIZE

section .text
global loader
loader:
	mov esp, kernel_stack + KERNEL_STACK_SIZE
	extern main
	call main

; In case main returns, enter an infinite loop.
.loop:
	hlt
	jmp .loop
