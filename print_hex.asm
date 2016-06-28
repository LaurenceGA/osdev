; This function takes a value in register ax and prints it in hexidecimal
print_hex:
	pusha
	mov bx, 5	; Initialise counter
hexloop:
	cmp bx, 2	; See if counter has finished
	jl endloop

	mov cx, 0xf	; extract the lowest 4 bits from argumnet
	and cx, ax

	cmp cx, 9
	jle modstr
	add cx, 7	; offset to letters

modstr:	add [hex_string+bx], cx	; Modify our hex_string
	shr ax, 4	; Move number down

	add bx, -1
	jmp hexloop

endloop:
	mov bx, hex_string
	call print_string

	; Now we need to reset the string so it can be used again
	mov bx, 5

resetloop:
	cmp bx, 2
	jl return

	mov byte [hex_string+bx], '0'	; Set to '0'

	add bx, -1
	jmp resetloop

return:
	popa
	ret
; Data
hex_string:	; Base hex string which we modify before printing
	db '0x0000', CR, LF, 0
