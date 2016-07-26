global loader		; Make the loader SubRoutine visible to the linker
global KSTACKSIZE

extern kmain		; We define the entry to the kernel in a c file
extern kernel_physical_end

MODULEALIGN	equ 1<<0	; Align loaded modules to page boundaries
MEMINFO		equ 1<<1	; We want GRUB to give us information regarding memory.
; We pass GRUB the above flags to tell it what we want.
FLAGS		equ MODULEALIGN | MEMINFO

; This is a special number for GRUB that allows it to find the multiboot header,
; defined in https://www.gnu.org/software/grub/manual/multiboot/multiboot.html
MAGIC		equ 0x1BADB002

CHECKSUM	equ -(MAGIC + FLAGS)	; This is required by GRUB.

KSTACKSIZE equ 0x4000	; Stack space to reserve for the kernel (16KB).

; This will be used to convert virtual addresses into physical addresses until we enable paging.
; This is not the virtual address of where the kernel itself is loaded, but the amount that
; must be subtracted from a virtual address to get it's physical address.
KVIRTUALBASE		equ 0xC0000000			; 3 GB
KPAGENUM		equ (KVIRTUALBASE >> 22)	; Page index of the kernel.

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
KERNEL_PT_CFG       equ 00000000000000000000000000001011b
KERNEL_PDT_ID_MAP   equ 00000000000000000000000010001011b

FOUR_KiB equ 0x1000

section .data
align 4096
kernelPT:
	; If this isn't present, then the kernel will crash immediately after paging
	; is enabled because it won't be able to fetch the next instruction.
	; It is okay to unmap this page at a later time if needed.
	times 1024 dd 0		; Pages before kernel space.
kernelPDT:
	; Define a 4MB page containing the kernel.
	dd KERNEL_PDT_ID_MAP
	times 1023 dd 0		; Pages after the kernel image.

; We'll use these later to store some information that grub gives us.
section .data
align 4
GRUB_MAGIC:
	dd 0
GRUB_MBI:
	dd 0

section .bss
align 32
stack:
	resb KSTACKSIZE

section .text
align 4
MultiBootHeader:
	dd MAGIC
	dd FLAGS
	dd CHECKSUM


; Were we will pick up when GRUB does its thing.
loader:
	; Store some information that GRUB has left for us to use.
	mov ecx, (GRUB_MAGIC - KVIRTUALBASE)	; The magic number it leaves us.
	mov [ecx], eax
	mov ecx, (GRUB_MBI - KVIRTUALBASE)	; The multiboot info it leaves us.
	mov [ecx], ebx

setupKernelPageDir:
	; Set up kernelPDT to point to kernelPT
	mov ecx, (kernelPDT - KVIRTUALBASE + KPAGENUM*4)
	mov edx, (kernelPT - KVIRTUALBASE)
	or  edx, KERNEL_PT_CFG
	mov [ecx], edx

setupKernelPageTable:
	mov eax, (kernelPT - KVIRTUALBASE)
	mov ecx, KERNEL_PT_CFG
.loop:
	mov [eax], ecx
	add eax, 4
	add ecx, FOUR_KiB
	cmp ecx, kernel_physical_end
	jle .loop

; Now lets enable paging.
enable_paging:
	mov ecx, (kernelPDT - KVIRTUALBASE)
	and ecx, 0xFFFFF000     ; The upper 20 bits are the only ones that matter.
	or  ecx, 0x08           ; Enable page write through, and then
	mov cr3, ecx            ; load pdt.

	mov ecx, cr4
	or  ecx, 0x00000010     ; We want 4MB pages, so we set the second bit
	mov cr4, ecx            ; and write that to cr4.

	mov ecx, cr0
	or  ecx, 0x80000000	; The highest bit in cr0 controls paging.
	mov cr0, ecx	        ; So, we enable it.

loadHigherHalf:
	; We can now start fetching instructions in the kernel space.
	; At this point, eip holds the physical address of the following command,
	; and we need to do a long jump to correct the virtual address of STARTKERNEL.
	lea ecx, [STARTKERNEL]
	jmp ecx			; This has to be an absolute jump.

; From here on out, we start using the page table.
STARTKERNEL:
	; We now unmap the identity-mapped first 4MB page of physical address space,
	; as we do not need it anymore.
	mov [kernelPDT], DWORD 0
	invlpg [0]

	; From here on out, paging is enabled. The first 4MB of physical address
	; space is mapped starting at KVIRTUALBASE. Everything else will be linked
	; to that address.
	mov esp, stack+KSTACKSIZE	; Set up the stack

	push kernelPT
	push kernelPDT

	push DWORD[GRUB_MAGIC]		; Pass the multiboot magic number

	; Now, we pass the Multiboot info into the main function that begins the
	; kernel.
	push DWORD[GRUB_MBI]

	call kmain
	hlt
