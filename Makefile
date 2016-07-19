# Important Directories
DRIVERDIR = drivers
KERNELDIR = kernel
BOOTDIR   = boot
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

# The name of the kernel.
KNAME = kernel

# The iso file
ISOFILE = $(KNAME).iso

# The kernel file that contains all the linked code.
LINKFILE  := $(ISODIR)/boot/$(KNAME).elf
LINKSCRIPT = link.ld

# File to write the disassembled version of the kernel to.
KDIS = $(KNAME).dis

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
AS      = nasm
ASFLAGS = -f elf

# C code is in the 2011 C standard. Must be 32 bit to be compatible
CC     = gcc
STD    = c11
CFLAGS = -std=$(STD) -m32 -Wall -Werror -Wpedantic -ffreestanding \
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

# I can't find a version of genisoimage that I can use on MacOS, so this is here
# to allow me to change what actually generates the iso image, without effecting
# anyone else running the program.
ISOGEN   = genisoimage
ISOFLAGS = -R -quiet -boot-info-table -no-emul-boot -boot-load-size 4 \
		   -input-charset utf8

# Our bootloader
GRUBDIR       = boot/grub
GRUB         := $(GRUBDIR)/stage2_eltorito
GRUBMENU     := $(ISODIR)/$(GRUBDIR)/menu.lst
GRUBTIMEOUT   = 1
GRUBDEFAULTS := default=0\ntimeout=$(GRUBTIMEOUT)



default: iso

# Just runs emu with our disk image
run: iso
	$(EMU) $(EMUFLAGS)

# It's very important that the dependencies are in this order so they are stuck
# together properly (entry before kernel)
$(LINKFILE): $(OBJFILES)
	$(LD) $(LDFLAGS) $^ -o $@ -T $(LINKSCRIPT)

# Compile C src files into their respective obj file.
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Assemble asm src files into their respective obj file.
%.o: %.asm
	$(AS) $< $(ASFLAGS) -o $@

# Disassemble our kernel - might be useful for debugging.
disassemble: $(LINKFILE)
	ndisasm -b 32 $< > $(KDIS)

iso: $(LINKFILE)
	echo "$(GRUBDEFAULTS)\n\ntitle $(KNAME)\nkernel /boot/$(KNAME).elf" > $(GRUBMENU)
	$(ISOGEN) $(ISOFLAGS) -b $(GRUB) -A $(KNAME) -o $(ISOFILE) $(ISODIR)

loc:
	@echo -e "Sorting files in ascending order of number of lines of code ..."
	@wc -l $(HEADERS) $(SRCFILES) $(ASMSRCFILES) $(LINKSCRIPT) $(GRUBMENU) \
		$(LOADER) | sort

# Remove all files that may have been built.
clean:
	$(RM) $(ISOFILE) $(LINKFILE) $(KDIS) $(OBJFILES)
