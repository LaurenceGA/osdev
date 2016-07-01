; This makes sure we jump straight into the kernel's entry function
[bits 32]
[extern main]	; Declare we will be referencing a variable elsewhere

call main	; Jump to our kernel entry point
jmp $
