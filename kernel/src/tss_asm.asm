global TSSLoadSet

section .text
TSSLoadSet:
	mov ax, [esp+4]     ; mov the tss segsel into ax (16 bits)
	ltr ax              ; load the task register with the selector
	ret
