CC=gcc
CFLAGS=-Wall -g -O0 -pedantic -std=c99 -D_POSIX_C_SOURCE
DEPS = acsilib.h taglib.h
.DEFAULT_GOAL := default 

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< 

main: main.o acsilib.o taglib.o
	$(CC) -o acsi main.c acsilib.c taglib.c
clean:
	rm -f *.o
tclean:
	rm -f i18n-*

default: main clean

test: test.o acsilib.o taglib.o
	$(CC) -o test test.c acsilib.c taglib.c

t: test

tc: tclean

all: main test clean
