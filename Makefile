AS=nasm
ASFILE=boot.asm
BINFILE=$(ASFILE:%.asm=%.bin)
ASFLAGS=-f bin -o $(BINFILE)

CC=gcc
CFLAGS=-c -Wall -Werror

LD=ld

EMU=qemu-system-i386
EMUFLAGS=-drive file=$(BINFILE),index=0,media=disk,format=raw

RM=rm -f

default: $(BINFILE)

$(BINFILE): $(ASFILE)
	$(AS) $(ASFILE) $(ASFLAGS)

run: $(BINFILE)
	$(EMU) $(EMUFLAGS)

clean:
	$(RM) $(BINFILE)
