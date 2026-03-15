CC = gcc
CFLAGS = -O2 -Wall -Wextra

life: life.c
	$(CC) $(CFLAGS) -o $@ $< -lm

clean:
	rm -f life

.PHONY: clean
