; This function takes a value in register ax and prints it in hexidecimal
print_hex:
	pusha
	mov bx, 5	; Initialise counter
hexloop:
	cmp bx, 2	; See if counter has finished
	jl endloop

	mov dx, hex_string	; Find the address we want
	add dx, bx

	mov cx, 0xf	; extract the lowest 4 bits from argumnet
	and cx, ax

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
