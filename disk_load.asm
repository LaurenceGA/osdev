; Load dh sectors to es:bx from drive dl
disk_load:
	pusha
	push dx	; So we can remember how many sectors we asked to read later

	mov ah, 0x02	; BIOS disk read sector function
	mov al, dh	; Number of sectors passed via dh
	mov ch, 0x00	; Cylinder 0
	mov dh, 0x00	; Head 0
	mov cl, 0x02	; Start at second sector (after boot sector)

	int 0x13	; Call read interrupt

	jc disk_error

	pop dx		; How many we requested
	cmp dh, al	; Did we read the right amount?
	jne disk_error	; Error if not

	popa
	ret		; Otherwise all is well

disk_error:
	mov bx, disk_error_msg
	call print_string
	jmp $	; hang

disk_error_msg:
	db "Disk read error!", CR, LF, 0
