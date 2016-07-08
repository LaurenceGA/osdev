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

# The image file that contains all os related code.
IMAGE = kernel_image

# The kernel file that contains all the linked code.
LINKFILE   = kernel.elf
LINKSCRIPT = link.ld

# File to write the disassembled version of the kernel to.
KDIS = kernel.dis

# The kernel loader
LOADER  := $(BOOTDIR)/loader.asm
LOADERO := $(LOADER:%.asm=%.o)

# Kernel related source files
KERNELSRCFILES := $(wildcard $(KERNELDIR)/*.c)
KERNELSRCFILES += $(wildcard $(KERNELSRCDIR)/*.c)
KERNELSRCFILES += $(wildcard $(KERNELSRCDIR)/*/*.c)
KERNELOBJFILES := $(KERNELSRCFILES:%.c=%.o)

# Kernel related assembly source files
KERNELASMSRCFILES := $(wildcard $(KERNELDIR)/*.asm)
KERNELASMSRCFILES += $(wildcard $(KERNELSRCDIR)/*.asm)
KERNELASMSRCFILES += $(wildcard $(KERNELSRCDIR)/*/*.asm)
KERNELASMOBJFILES := $(KERNELASMSRCFILES:%.asm=%.o)

# Driver related source files
DRIVERSRCFILES := $(wildcard $(DRIVERDIR)/*.c)
DRIVERSRCFILES += $(wildcard $(DRIVERSRCDIR)/*.c)
DRIVERSRCFILES += $(wildcard $(DRIVERSRCDIR)/*/*.c)
DRIVEROBJFILES := $(DRIVERSRCFILES:%.c=%.o)

# Driver related assembly source files
DRIVERASMSRCFILES := $(wildcard $(DRIVERDIR)/*.asm)
DRIVERASMSRCFILES += $(wildcard $(DRIVERSRCDIR)/*.asm)
DRIVERASMSRCFILES += $(wildcard $(DRIVERSRCDIR)/*/*.asm)
DRIVERASMOBJFILES := $(DRIVERASMSRCFILES:%.asm=%.o)

# Libc related sourced files
LIBCSRCFILES := $(wildcard $(LIBCDIR)/*.c)
LIBCSRCFILES += $(wildcard $(LIBCSRCDIR)/*.c)
LIBCSRCFILES += $(wildcard $(LIBCSRCDIR)/*/*.c)
LIBCOBJFILES := $(LIBCSRCFILES:%.c=%.o)

# Header files
HEADERS := $(wildcard $(LIBCINCLUDESDIR)/*.h)
HEADERS += $(wildcard $(LIBCINCLUDESDIR)/*/*.h)
HEADERS += $(wildcard $(KERNELINCLUDESDIR)/*.h)
HEADERS += $(wildcard $(DRIVERINCLUDESDIR)/*.h)
HEADERS += $(wildcard $(KERNELINCLUDESDIR)/*/*.h)
HEADERS += $(wildcard $(DRIVERINCLUDESDIR)/*/*.h)

# Flags let us assemble to flat binary
AS       = nasm
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


default: iso

# Just runs emu with our disk image
run: iso
	$(EMU) $(EMUFLAGS)

# It's very important that the dependencies are in this order so they are stuck
# together properly (entry before kernel)
$(LINKFILE): $(DRIVEROBJFILES) $(LIBCOBJFILES) $(KERNELOBJFILES)
$(LINKFILE): $(LOADERO) $(DRIVERASMOBJFILES) $(KERNELASMOBJFILES)
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
	cp $< iso/boot/$<
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
	$(RM) $(DRIVEROBJFILES) $(DRIVERASMOBJFILES) $(KERNELOBJFILES) $(ISOFILE)
	$(RM) $(LIBCOBJFILES) $(LINKFILE) $(LOADERO) $(KDIS) $(LINKFILE)
	$(RM) $(KERNELASMOBJFILES)

