; Loads the interrupt descriptor table
; stack: [esp + 4] address of the first entry of the IDT
;	 [esp] return address
global load_idt
load_idt:
	mov eax, [esp + 4]
	lidt [eax]
	ret
