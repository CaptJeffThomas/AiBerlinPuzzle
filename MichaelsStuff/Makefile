
SHELL=/bin/bash
CC=gcc
CFLAGS=-std=c99 -Wall -g


setup: main.c func.o 
	$(CC) $(CFLAGS) -o $@ $^ -lm

%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $< -lm

clean:
	$(RM) -f *~ *# *.o setup