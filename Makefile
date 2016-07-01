
# Assembler of chouce
AS      = nasm
# Assemble to flat binary
ASFLAGS = -f bin

# Boot sector code
BOOT    = boot.asm
ASFILES := $(wildcard ./*/*.asm)
ASFILES += $(wildcard ./*.asm)
BINFILE  = $(BOOT:%.asm=%.bin)

IMAGE = os_image

# C code is compiled using gcc with the C standard of 2011
# It's importan that it's in 32 bit mode to be compatible with our os
CC     = gcc
STD    = c11
CFLAGS = -c -std=$(STD) -m32 -Wall -Werror -ffreestanding

LD      = ld
LDFLAGS = -m elf_i386 --oformat binary --entry main -Ttext 0x1000

# Qemu is the cpu emulator used. The flags ensure it knows what kind of
# disk image it's getting. Without them is gives a warning
# EMU := $(shell command -v qemu-system-i386 >>/dev/null)
EMU      = qemu-system-i386
EMUFLAGS = -drive file=$(IMAGE),index=0,media=disk,format=raw

# the -f options suppresses warnings if a file is not present
RM=rm -f

# ifndef EMU
	# $(error "QEMU doesn't appear to be installed")
# endif

# No target specified, so just create the OS image.
default: $(IMAGE)

# Compilation of our boot sector
$(BINFILE): $(ASFILES)
	$(AS) $(ASFLAGS) $(BOOT) -o $(BINFILE)

# Just runs emu with our disk image
run: $(IMAGE)
	$(EMU) $(EMUFLAGS)

# Sticks our component binaries (boot sector, kernel and extra space) together
# to create our disk image
$(IMAGE): $(BINFILE) kernel.bin disk_space.bin
	cat $^ > $@

# Just out extra space padding. Without this, if we tried to read too much
# we would throw an error
disk_space.bin: nullbytes.asm
	$(AS) $(ASFLAGS) $< -o $@

# It's very important that the dependencies are in this order so they are stuck
# together properly (entry before kernel)
# This compiles our kernel. It's also important that it's in i386 mode format
# compatability with our other code
# The linker looks for _start, but we don't have one so we let say
kernel.bin: kernel_entry.o kernel.o
	# --entry main so it knows where our start point is (main function)
	$(LD) $(LDFLAGS) -o $@ $^

# Create the object of our main kernel code
kernel.o: kernel.c
	$(CC) $(CFLAGS) -c $< -o $@

# Entry file that ensures we just straight into our kernel's main method
kernel_entry.o: kernel_entry.asm
	$(AS) $< -f elf -o $@

# Remove all but source files
clean:
	$(RM) *.o *.bin os-image *.dis

# Disassemble our kernel - might be useful for debugging .
kernel.dis: kernel.bin
	ndisasm -b 32 $< > $@
