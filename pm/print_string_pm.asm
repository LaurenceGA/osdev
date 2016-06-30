[bits 32]
; Some constants
VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f

; Prints out a string in protected mode.
; Null-terminated string location passed through EBX
print_string_pm:
	pusha	; Save registers
	mov edx, VIDEO_MEMORY		; Save start of vid mem to edx
print_loop:
	cmp byte [ebx], 0	; Reached null terminator?
	je pm_end

	mov al, [ebx]		; Put our char into al
	mov ah, WHITE_ON_BLACK; Put attributes into ah

	mov [edx], ax		; Store them at current cell

	add ebx, 1		; Move to next char
	add edx, 2		; Move to next charachter cell in vid mem
	jmp print_loop
pm_end:
	popa	; restore registers
	ret
