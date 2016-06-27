; Prints out a string. String location passed through bx
print_string:
	pusha	; Save registers
	mov ah, 0x0e	; BIOS tele-type output
loop:
	cmp byte [bx], 0	; Reached null terminator?
	je end

	mov al, [bx]		; Print our char
	int 0x10

	add bx, 1		; Move to next char
	jmp loop
end:
	popa	; restore registers
	ret
