CC = gcc
CFLAGS = -O2 -Wall

life: life.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f life

.PHONY: clean
