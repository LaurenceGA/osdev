# Important Directories
BOOTDIR   = boot
KERNELDIR = kernel
DRIVERDIR = drivers
LIBCDIR   = libc
ISODIR    = iso

# Kernel source and include directories
KERNELINCLUDESDIR := $(KERNELDIR)/includes
KERNELSRCDIR      := $(KERNELDIR)/src

# Driver source and include directories
DRIVERINCLUDESDIR := $(DRIVERDIR)/includes
DRIVERSRCDIR      := $(DRIVERDIR)/src

# Libc source and include directories
LIBCINCLUDESDIR   := $(LIBCDIR)/includes
LIBCSRCDIR        := $(LIBCDIR)/src

# The iso file
ISOFILE = kernel.iso

# The kernel file that contains all the linked code.
LINKFILE   = kernel.elf
LINKSCRIPT = link.ld

# File to write the disassembled version of the kernel to.
KDIS = kernel.dis

# The kernel loader
LOADER := $(BOOTDIR)/loader.asm

# Kernel related source files
SRCFILES := $(wildcard $(KERNELDIR)/*.c $(KERNELSRCDIR)/*.c $(KERNELSRCDIR)/*/*.c)

# Driver related source files
SRCFILES += $(wildcard $(DRIVERDIR)/*.c $(DRIVERSRCDIR)/*.c $(DRIVERSRCDIR)/*/*.c)

# Libc related sourced files
SRCFILES += $(wildcard $(LIBCDIR)/*.c $(LIBCSRCDIR)/*.c $(LIBCSRCDIR)/*/*.c)

# Kernel related assembly source files
ASMSRCFILES := $(wildcard $(KERNELDIR)/*.asm $(KERNELSRCDIR)/*.asm $(KERNELSRCDIR)/*/*.asm)

# Driver related assembly source files
ASMSRCFILES += $(wildcard $(DRIVERDIR)/*.asm $(DRIVERSRCDIR)/*.asm $(DRIVERSRCDIR)/*/*.asm)

# All object files
OBJFILES := $(LOADER:%.asm=%.o)
OBJFILES += $(ASMSRCFILES:%.asm=%.o)
OBJFILES += $(SRCFILES:%.c=%.o)

# Header files
HEADERS := $(wildcard $(LIBCINCLUDESDIR)/*.h   $(LIBCINCLUDESDIR)/*/*.h)
HEADERS += $(wildcard $(KERNELINCLUDESDIR)/*.h $(KERNELINCLUDESDIR)/*/*.h)
HEADERS += $(wildcard $(DRIVERINCLUDESDIR)/*.h $(DRIVERINCLUDESDIR)/*/*.h)

# Flags let us assemble to flat binary
AS       = nasm
ASFLAGS = -f elf

# C code is in the 2011 C standard. Must be 32 bit to be compatible
CC     = gcc
STD    = c11
CFLAGS = -std=$(STD) -m32 -Wall -Werror -ffreestanding \
         -I$(KERNELINCLUDESDIR) -I$(DRIVERINCLUDESDIR) -I$(LIBCINCLUDESDIR)

# The linker w'll use. --entry main so it knows where our start point is (main
# function).
LD      = ld
LDFLAGS = -m elf_i386

# The flags ensure it knows what kind of disk image it's getting.
EMU      = qemu-system-i386
EMUFLAGS = -cdrom $(ISOFILE)

# The -f options suppresses warnings if a file is not present
RM = rm -f



default: iso

# Just runs emu with our disk image
run: iso
	$(EMU) $(EMUFLAGS)

# It's very important that the dependencies are in this order so they are stuck
# together properly (entry before kernel)
$(LINKFILE): $(OBJFILES)
	ld $(LDFLAGS) $^ -o $@ -T $(LINKSCRIPT)

# Compile C src files into their respective obj file.
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Assemble asm src files into their respective obj file.
%.o: %.asm
	$(AS) $< $(ASFLAGS) -o $@

# Disassemble our kernel - might be useful for debugging .
disassemble: $(LINKFILE)
	ndisasm -b 32 $< > $(KDIS)

iso: $(LINKFILE)
	cp $< $(ISODIR)/boot/$<
	genisoimage -R                              \
                -b boot/grub/stage2_eltorito    \
                -no-emul-boot                   \
                -boot-load-size 4               \
                -A os                           \
                -input-charset utf8             \
                -quiet                          \
                -boot-info-table                \
                -o $(ISOFILE)                   \
                $(ISODIR)

# Remove all but source files
clean:
	$(RM) $(ISOFILE) $(LINKFILE) $(KDIS) $(OBJFILES)

