# Assembler of chouce
AS=nasm
# Assemble to flat binary
ASFLAGS = -f bin

# Boot sector code
BOOTDIR		= boot
BOOT		= $(BOOTDIR)/boot.asm
BOOTASFILES	= $(wildcard $(BOOTDIR)/*/*.asm)
# ASFILES := $(wildcard ./*/*.asm)
# ASFILES += $(wildcard ./*.asm)
BOOTBIN		= $(BOOT:%.asm=%.bin)

# C code is compiled using gcc with the C standard of 2011
# It's importan that it's in 32 bit mode to be compatible with our os
CC=gcc
STD=c11
CFLAGS=-c -std=$(STD) -m32 -Wall -Werror -Idrivers/
CSOURCES = $(wildcard drivers/src/*.c kernel/src/*.c libc/src/*.c)
HEADERS = $(wildcard drivers/include/*.h kernel/include/*.h libc/include/*.h)
OBJS = $(CSOURCES:%.c=%.o)
OBJS += drivers/src/io.o	# Compiled from assembly, not C

LD=ld

# Qemu is the cpu emulator used. The flags ensure it knows what kind of
# disk image it's getting. Without them is gives a warning
# EMU := $(shell command -v qemu-system-i386 >>/dev/null)
EMU = qemu-system-i386
EMUFLAGS=-drive file=os-image,index=0,media=disk,format=raw

RM=rm -f

# Create OS image by default.
default: os-image

# Just runs emu with our disk image
run: os-image
	$(EMU) $(EMUFLAGS)

# Compilation of our boot sector
$(BOOTBIN): $(BOOT) $(BOOTASFILES)
	$(AS) $(ASFLAGS) $< -I $(BOOTDIR)/ -o $@

# Sticks our component binaries together
os-image: $(BOOTBIN) kernel.bin disk_space.bin
	cat $^ > $@

disk_space.bin: $(BOOTDIR)/nullbytes.asm
	$(AS) $(ASFLAGS) $< -o $@

# kernel_entry must be first
kernel.bin: kernel/kernel_entry.o $(OBJS)
	$(LD) -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary --entry main

%.o: %.c $(HEADERS)
	$(CC) -ffreestanding $(CFLAGS) -c $< -o $@

%.o: %.asm
	$(AS) $< -f elf -o $@

clean:
	$(RM) os-image *.dis
	find . -type f -name '*.o' -delete
	find . -type f -name '*.bin' -delete

# Disassemble our kernel
kernel.dis: kernel.bin
	ndisasm -b 32 $< > $@
