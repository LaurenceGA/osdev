AS=nasm
ASFLAGS = -f bin

BOOT     = boot_sect.asm
ASFILES := $(wildcard ./*/*.asm)
ASFILES += $(wildcard ./*.asm)
BINFILE  = $(ASFILE:%.asm=%.bin)

CC=gcc
CFLAGS=-c -Wall -Werror

LD=ld

EMU := $(shell command -v qemu-system-i386 >>/dev/null)
EMUFLAGS=-drive file=$(BINFILE),index=0,media=disk,format=raw

RM=rm -f

ifndef EMU
	$(error "QEMU doesn't appear to be installed")
else
	EMU = qemu-system-i386
endif

RM = rm

# No target specified, so just assemble the file.
default: $(BINFILE)

$(BINFILE): $(ASFILES)
	$(AS) $(BOOT) -o $(BINFILE)

run: $(BINFILE)
	$(EMU) $(EMUFLAGS)

clean:
	$(RM) $(BINFILE)
