AS = nasm

ASFILE  = boot.asm
BINFILE = $(ASFILE:%.asm=%.bin)

ASFLAGS = -f bin -o $(BINFILE)


EMU := $(shell command -v qemu-system-i386 2>/dev/null)
EMUFLAGS = -drive file=$(BINFILE),index=0,media=disk,format=raw

ifndef EMU
	$(error "QEMU doesn't appear to be installed")
else
	EMU = qemu-system-i386
endif
RM = rm

# If no target is specified, then build the project.
default: build

build:
	$(AS) $(ASFILE) $(ASFLAGS)

run: build
	$(EMU) $(EMUFLAGS)

clean:
	$(RM) $(BINFILE)

