extern interruptHandler

global enableInterrupts

%macro no_error_code_handler 1
global interruptHandler_%1
interruptHandler_%1:
	push dword 0	; 0 for the error code
	push dword %1	; interrupt number
	jmp common_interrupt_handler
%endmacro

%macro error_code_handler 1
global interruptHandler_%1
interruptHandler_%1:
	push dword %1	; interrupt number
	jmp common_interrupt_handler
%endmacro

common_interrupt_handler:
	push esp
	add dword [esp], 8
	; save registers
	push eax
	push ebx
	push ecx
	push edx
	push ebp
	push esi
	push edi

	call interruptHandler	; Call C function

	; Restore registers
	pop edi
	pop esi
	pop ebp
	pop edx
	pop ecx
	pop ebx
	pop eax

	; restore esp (from error code and interrupt number added before)
	pop esp
	iret

enableInterrupts:
	sti
	ret

; protected mode exceptions
no_error_code_handler 0
no_error_code_handler 1
no_error_code_handler 2
no_error_code_handler 3
no_error_code_handler 4
no_error_code_handler 5
no_error_code_handler 6
no_error_code_handler 7
error_code_handler 8
no_error_code_handler 9
error_code_handler 10
error_code_handler 11
error_code_handler 12
error_code_handler 13
error_code_handler 14
no_error_code_handler 15
no_error_code_handler 16
error_code_handler 17
no_error_code_handler 18
no_error_code_handler 19

; IRQs
no_error_code_handler 32
no_error_code_handler 33
no_error_code_handler 34
no_error_code_handler 35
no_error_code_handler 36
no_error_code_handler 37
no_error_code_handler 38
no_error_code_handler 39
no_error_code_handler 40
no_error_code_handler 41
no_error_code_handler 42
no_error_code_handler 43
no_error_code_handler 44
no_error_code_handler 45
no_error_code_handler 46
no_error_code_handler 47
