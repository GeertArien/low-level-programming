CC = gcc
CFLAGS = -std=c11 -Wall
LD = gcc

all: app

app: app.o math.o list.o algorithm.o for_all.o
	$(LD) $^ -o $@

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o app

.PHONY: clean