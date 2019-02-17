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
rt:
	rm -f i18n-*

default: main clean

test: test.o acsilib.o taglib.o
	$(CC) -o test test.c acsilib.c taglib.c

t: test

tc: test clean

help:
	@echo 'build: main.o acsilib.o taglib.o\n\t$(CC) -o acsi main.c acsilib.c taglib.c\nclean:\n\trm -f *.o\nrt:\n\trm -f i18n-*\ncb: build clean\ntest: test.o acsilib.o taglib.o\n\t$(CC) -o test test.c acsilib.c taglib.c\nt: test clean\nall: build test clean'

all: main test clean
