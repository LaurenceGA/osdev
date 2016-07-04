# Important Directories
BOOTDIR   = boot
KERNELDIR = kernel
DRIVERDIR = drivers

# Kernel source and include directories
KERNELINCLUDEDIR := $(KERNELDIR)/includes
KERNELSRCDIR     := $(KERNELDIR)/src

# Driver source and include directories
DRIVERINCLUDEDIR := $(DRIVERDIR)/includes
DRIVERSRCDIR     := $(DRIVERDIR)/src

# Boot related code
BOOTFILE  := $(BOOTDIR)/boot.asm
ASFILES   := $(wildcard $(BOOTDIR)/*.asm)
ASFILES   += $(wildcard $(BOOTDIR)/*/*.asm)
BOOTBIN   := $(BOOTFILE:%.asm=%.bin)
BINFILES  := $(ASFILES:%.asm=%.bin)
DISKSPACE := diskspace.bin

# Kernel related code
KERNEL    := $(KERNELDIR)/kernel_entry.asm
KERNELO   := $(KERNEL:%.asm=%.o)

# Kernel related source files
KERNELSRCFILES := $(wildcard $(KERNELDIR)/*.c)
KERNELSRCFILES += $(wildcard $(KERNELSRCDIR)/*.c)
KERNELSRCFILES += $(wildcard $(KERNELSRCDIR)/*/*.c)
KERNELOBJFILES := $(KERNELSRCFILES:%.c=%.o)

# Driver related source files
DRIVERSRCFILES := $(wildcard $(DRIVERDIR)/*.c)
DRIVERSRCFILES += $(wildcard $(DRIVERSRCDIR)/*.c)
DRIVERSRCFILES += $(wildcard $(DRIVERSRCDIR)/*/*.c)
DRIVEROBJFILES := $(DRIVERSRCFILES:%.c=%.o)

# Header files
HEADERS := $(wildcard $(KERNELINCLUDEDIR)/*.c)
HEADERS += $(wildcard $(DRIVERINCLUDEDIR)/*.c)
HEADERS += $(wildcard $(KERNELINCLUDEDIR)/*/*.c)
HEADERS += $(wildcard $(DRIVERINCLUDEDIR)/*/*.c)

# The image file that contains all os related code.
IMAGE = kernel_image

# The kernel file that contains all the linked code.
LINKFILE = link.bin

# File to write the disassembled version of the kernel to.
KDIS = kernel.dis


# Assembler of choice. Flags let us assemble to flat binary.
AS      = nasm
ASFLAGS = -f bin -I$(BOOTDIR)/

# C code is compiled using gcc with the C standard of 2011.
# It's importan that it's in 32 bit mode to be compatible with our os.
CC     = gcc
STD    = c11
CFLAGS = -std=$(STD) -m32 -Wall -Werror -Wpedantic -ffreestanding \
	-I$(KERNELINCLUDEDIR) -I$(DRIVERINCLUDEDIR)

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
	$(AS) $(ASFLAGS) $(BOOTFILE) -o $@

# Just runs emu with our disk image
run: $(IMAGE)
	$(EMU) $(EMUFLAGS)

# Sticks our component binaries (boot sector, kernel and extra space) together
# to create our disk image
$(IMAGE): $(BOOTBIN) $(LINKFILE) $(DISKSPACE)
	cat $^ > $@

# Just out extra space padding. Without this, if we tried to read too much
# we would throw an error
$(DISKSPACE): $(BOOTDIR)/nullbytes.asm
	$(AS) $(ASFLAGS) $< -o $@

# It's very important that the dependencies are in this order so they are stuck
# together properly (entry before kernel)
$(LINKFILE): $(KERNELO) $(KERNELOBJFILES) $(DRIVEROBJFILES)
	$(LD) $(LDFLAGS) -o $@ $^

# Create the object of our main kernel code
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Entry file that ensures we just straight into our kernel's main method
$(KERNELO): $(KERNEL)
	$(AS) $< -f elf -o $@

# Disassemble our kernel - might be useful for debugging .
disassemble: $(LINKFILE)
	ndisasm -b 32 $< > $(KDIS)

# Remove all but source files
clean:
	$(RM) $(DRIVEROBJFILES) $(DISKSPACE) $(KERNELO) $(IMAGE)
	$(RM) $(KERNELOBJFILES) $(LINKFILE)  $(BOOTBIN)  $(KDIS)
