; Loads the interrupt descriptor table
; stack: [esp + 4] address of the first entry of the IDT
;	 [esp] return address
global loadIDT
loadIDT:
	mov eax, [esp + 4]
	lidt [eax]
	ret
