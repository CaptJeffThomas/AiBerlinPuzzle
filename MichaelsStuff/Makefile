
SHELL=/bin/bash
CC=gcc
CFLAGS=-Wall -g -std=c99


setup: main.c func.o 
	$(CC) $(CFLAGS) -o $@ $^ -lm

%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $< -lm

clean:
	$(RM) -f *~ *# *.o setup