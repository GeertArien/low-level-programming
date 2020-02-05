AS = nasm
LD = ld
ASFLAGS = -f elf64

program: itc.o
	$(LD) -o program itc.o

itc.o: itc.asm
	$(AS) $(ASFLAGS) -o itc.o itc.asm

clean:
	rm itc.o program

help:
	echo 'This is the help'

.PHONY: clean