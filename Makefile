AS = nasm

ASFILE  = boot.asm
BINFILE = $(ASFILE:%.asm=%.bin)

ASFLAGS = -f bin -o $(BINFILE)

EMU := $(shell command -v qemu 2>/dev/null)
EMUFLAGS = -drive file=$(BINFILE),index=0,media=disk,format=raw

ifndef EMU
	EMU = qemu-system-i386
else
	EMU = qemu
endif

RM = rm

default: build

build:
	$(AS) $(ASFILE) $(ASFLAGS)

run: build
	$(EMU) $(EMUFLAGS)

clean:
	$(RM) $(BINFILE)

