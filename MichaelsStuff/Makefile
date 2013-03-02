#*****************************************************************
# NAME: Croydon Enders, Michael Hnatiw, Jeff Thomas
# CLASS/ASSIGNMENT: CMPT355 Project #1
# PROF: Calin Anton
#*****************************************************************

# *** MACRO VARIABLES ***
SHELL=/bin/bash
CC=gcc
CFLAGS=-std=c99 -Wall -g -D_POSIX_SOURCE

# target used to compile AB game program 
AB: main.c global.o fringe.o closed.o
	$(CC) $(CFLAGS) -o $@ $^ -lm

# use of wildcards to compile all object files for parser
%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $< -lm

# command to remove all unwanted files, object, and executables as well
clean:
	$(RM) -f *~ *# *.o AB
