global _loader		; Make the loader SubRoutine visible to the linker
extern main		; We define the entry to the kernel in a c file

MODULEALIGN	equ 1<<0	; Align loaded modules to page boundaries
MEMINFO		equ 1<<1	; We want GRUB to give us information regarding memory.
; We pass GRUB the above flags to tell it what we want.
FLAGS		equ MODULEALIGN | MEMINFO

; This is a special number for GRUB that allows it to find the multiboot header,
; defined in https://www.gnu.org/software/grub/manual/multiboot/multiboot.html
MAGIC		equ 0x1BADB002

CHECKSUM	equ -(MAGIC + FLAGS)	; This is required by GRUB.

; This will be used to convert virtual addresses into physical addresses until we enable paging.
; This is not the virtual address of where the kernel itself is loaded, but the amount that
; must be subtracted from a virtual address to get it's physical address.
KVIRTUALBASE		equ 0xC0000000			; 3 GB
KPAGENUM		equ (KVIRTUALBASE >> 22)	; Page index of the kernel.

section .data
align 0x1000
BootPageDirectory:
	; We want to identity-map the first 4 MB of the 32-bit physical address space.
	; Bits: (With 4MB pages, bits 21-12 are reserved.)
	; 31 - 12: Page table 4-kb aligned address
	; 11 -  9: Available
	; 8: Ignored
	; 7: Page Size (0 for 4KB, 1 for 4MB)
	; 6: Always 0
	; 5: Whether a page has been read or written to. 1 -- Yes 0 -- No.
	; 4: Whether to cache the page or not. 1 -- No Cache 0 -- Cache.
	; 3: Controls the Write-through ability of the page. 1 -- Enabled 0 -- Disabled
	; 2: Priviledge Level. 1 -- User 0 -- Supervisor
	; 1: Read/Write permissions. 1 -- R/W Enabled 0 -- R/W Disabled
	; 0: If this bit is set, the page is in physical memory.
	; If this isn't present, then the kernel will crash immediately after paging
	; is enabled because it won't be able to fetch the next instruction.
	; It is okay to unmap this page at a later time if needed.
	dd 0x00000083				; We want 4MB pages, R/W Enabled, Page present
	times (KPAGENUM - 1) dd 0		; Pages before kernel space.
	; Define a 4MB page containing the kernel.
	dd 0x00000083				; Same as above.
	times (1024 - KPAGENUM - 1) dd 0	; Pages after the kernel image.

section .text
align 4
MultiBootHeader:
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

KSTACKSIZE equ 0x4000	; Stack space to reserve for the kernel (16KB).

; The entry point for the linker
loader equ (_loader - 0xC0000000)
global loader


_loader:
	; Until Paging is set up, we need to use physical address, and not virtual ones.
	mov ecx, (BootPageDirectory - KVIRTUALBASE)
	mov cr3, ecx		; Load the Page Directory Base Register.

	mov ecx, cr4
	or ecx, 0x00000010	; Set the PSE bit in cr4 to enable 4MB pages
	mov cr4, ecx

	mov ecx, cr0
	or ecx, 0x80000000	; Set PG bit in cr0 to enable paging.
	mov cr0, ecx

	; We can now start fetching instructions in the kernel space.
	; At this point, eip holds the physical address of the following command,
	; and we need to do a long jump to correct the virtual address of STARTKERNEL.
	lea ecx, [STARTKERNEL]
	jmp ecx			; This has to be an absolute jump.

STARTKERNEL:
	; We now unmap the identity-mapped first 4MB page of physical address space,
	; as we do not need it anymore.
	mov dword [BootPageDirectory], 0
	invlpg [0]

	; From here on out, paging is enabled. The first 4MB of physical address
	; space is mapped starting at KVIRTUALBASE. Everything else will be linked
	; to that address.
	mov esp, stack+KSTACKSIZE	; Set up the stack

	push eax			; Pass the multiboot magic number

	; Now, we pass the Multiboot info into the main function that begins the
	; kernel. This is a physical address, and may not be in the first 4MB.
	add ebx, KVIRTUALBASE		; Map the multiboot info to the correct address.
	push ebx

	call main
	hlt

section .bss
align 32
stack:
	resb KSTACKSIZE
