
SHELL=/bin/bash
CC=gcc
CFLAGS=-std=c99 -Wall -g -D_POSIX_SOURCE


AB: main.c func.o hash.o
	$(CC) $(CFLAGS) -o $@ $^ -lm

%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $< -lm

clean:
	$(RM) -f *~ *# *.o AB
