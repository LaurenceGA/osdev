; This function takes a value in register ax and prints it in hexidecimal
print_hex:
	pusha
	mov bx, 5	; Initialise counter
hexloop:
	cmp bx, 2	; See if counter has finished
	jl endloop

	mov cx, 0xf	; extract the lowest 4 bits from argumnet
	and cx, ax

	cmp cx, 9	; See if it should be a letter or digit
	jle modstr
	add cx, 7	; offset to letters if needed

modstr:
	mov byte [hex_string+bx], '0'	; First set it to 0
	add [hex_string+bx], cx	; Modify our hex_string
	shr ax, 4	; Move number down

	add bx, -1
	jmp hexloop

endloop:
	mov bx, hex_string
	call print_string

	popa
	ret
; Data
hex_string:	; Base hex string which we modify before printing
	db '0x0000', CR, LF, 0