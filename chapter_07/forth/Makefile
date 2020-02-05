AS = nasm
LD = ld
ASFLAGS = -f elf64

program: main.o lib.o dict.o
	$(LD) -o program main.o lib.o dict.o

main.o: main.asm
	$(AS) $(ASFLAGS) -o main.o main.asm

lib.o: lib.asm
	$(AS) $(ASFLAGS) -o lib.o lib.asm

dict.o: dict.asm
	$(AS) $(ASFLAGS) -o dict.o dict.asm

clean:
	rm main.o lib.o dict.o program

help:
	echo 'This is the help'

.PHONY: clean