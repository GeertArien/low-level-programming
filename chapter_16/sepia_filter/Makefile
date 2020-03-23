CC = gcc
CFLAGS = -std=c11 -Wall
LD = gcc

all: app

app: app.o bmp.o image.o sse.o
	$(LD) $^ -o $@ -lm

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

sse.o: sse.asm
	nasm -felf64 sse.asm -o sse.o

clean:
	rm -f *.o app

.PHONY: clean