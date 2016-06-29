AS = nasm

BOOT     = boot.asm
ASFILES := $(wildcard ./*/*.asm)
ASFILES += $(wildcard ./*.asm)
BINFILE  = $(ASFILE:%.asm=%.bin)

ASFLAGS = -f bin

EMU := $(shell command -v qemu-system-i386 2>/dev/null)
EMUFLAGS = -drive file=$(BINFILE),index=0,media=disk,format=raw

ifndef EMU
	$(error "QEMU doesn't appear to be installed")
else
	EMU = qemu-system-i386
endif

RM = rm

# No target specified, so just assemble the file.
default: $(BINFILE)

$(BINFILE): $(BOOT) $(ASFILES)
	$(AS) $< -o $@

run: $(BINFILE)
	$(EMU) $(EMUFLAGS)

clean:
	$(RM) $(BINFILE)

