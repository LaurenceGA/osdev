global outb, inb	; Make the labels visible

; outb - send byte to i/o port
; stack:[esp + 8] data byte
;	[esp + 4] i/o port
;	[esp	] return address
outb:
	mov al, [esp + 8]	; move data to be sent into al
	mov dx, [esp + 4]	; move port address into dx
	out dx, al		; send the data to the port
	ret

; inb - read byte from i/o port
; stack:[esp + 4] i/o port
;	[esp	] return address
inb:
	mov dx, [esp + 4]	; move port address into dx
	in al, dx		; Read in the data from the port
	ret

