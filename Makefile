# Important Directories
BOOTDIR = boot/
KDIR    = kernel/
INCDIR := $(KDIR)includes/
SRCDIR := $(KDIR)src/

# Boot sector code
BOOT     := $(BOOTDIR)boot.asm
ASFILES  := $(wildcard $(BOOTDIR)*.asm)
ASFILES  += $(wildcard $(BOOTDIR)real/*.asm)
ASFILES  += $(wildcard $(BOOTDIR)protected/*.asm)
BOOTBIN  := $(BOOT:%.asm=%.bin)
BINFILES := $(ASFILES:%.asm=%.bin)

# Kernel sector code
KERNEL   := $(KDIR)kernel_entry.asm
KERNELO  := $(KERNEL:%.asm=%.o)
SRCFILES := $(wildcard $(KDIR)*.c)
SRCFILES += $(wildcard $(SRCDIR)*.c)
SRCFILES += $(wildcard $(SRCDIR)*/*.c)
OBJFILES := $(SRCFILES:%.c=%.o)

# Assembler of choice. Flags let us assemble to flat binary.
AS      = nasm
ASFLAGS = -f bin -I$(BOOTDIR)

# The image file that contains all os related code.
IMAGE = os_image

# C code is compiled using gcc with the C standard of 2011.
# It's importan that it's in 32 bit mode to be compatible with our os.
CC     = gcc
STD    = c11
CFLAGS = -std=$(STD) -m32 -Wall -Werror -ffreestanding -I$(INCDIR)

# The linker w'll use. --entry main so it knows where our start point is (main
# function).
LD      = ld
LDFLAGS = -m elf_i386 --oformat binary --entry main -Ttext 0x1000

# Qemu is the cpu emulator used. The flags ensure it knows what kind of
# disk image it's getting. Without them is gives a warning
# EMU := $(shell command -v qemu-system-i386 >>/dev/null)
EMU      = qemu-system-i386
EMUFLAGS = -drive file=$(IMAGE),index=0,media=disk,format=raw

# the -f options suppresses warnings if a file is not present
RM=rm -f



# No target specified, so just create the OS image.
default: $(IMAGE)

# Compilation of our boot sector
$(BOOTBIN): $(ASFILES)
	$(AS) $(ASFLAGS) $(BOOT) -o $(BOOTBIN)

# Just runs emu with our disk image
run: $(IMAGE)
	$(EMU) $(EMUFLAGS)

# Sticks our component binaries (boot sector, kernel and extra space) together
# to create our disk image
$(IMAGE): $(BOOTBIN) kernel_and_link disk_space.bin
	cat $^ > $@

# Just out extra space padding. Without this, if we tried to read too much
# we would throw an error
disk_space.bin: $(BOOTDIR)nullbytes.asm
	$(AS) $(ASFLAGS) $< -o $@

# It's very important that the dependencies are in this order so they are stuck
# together properly (entry before kernel)
kernel_and_link: $(KERNELO) $(OBJFILES)
	@echo "Linking the following: $^"
	$(LD) $(LDFLAGS) -o $@ $^

# Create the object of our main kernel code
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Entry file that ensures we just straight into our kernel's main method
$(KERNELO): $(KERNEL)
	$(AS) $< -f elf -o $@

# Remove all but source files
clean:
	$(RM) *.o *.bin $(IMAGE) *.dis $(BOOTBIN) $(KERNELO) $(OBJFILES)

# Disassemble our kernel - might be useful for debugging .
kernel.dis: kernel
	ndisasm -b 32 $< > $@
