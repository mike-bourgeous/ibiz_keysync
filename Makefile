.PHONY: all clean

CFLAGS=-Wall -Wextra -g -std=c99 -D_XOPEN_SOURCE=700 -O2
CC=gcc

all: keysync

clean:
	rm -f keysync

keysync: keysync.c
	$(CC) $(CFLAGS) keysync.c -o keysync
