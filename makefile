CC=gcc
CFLAGS=-Wall -g -O0 -pedantic -std=c99 -D_POSIX_C_SOURCE
DEPS = src/acsilib.h src/taglib.h

.DEFAULT_GOAL := default 

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< 

main: src/main.o src/acsilib.o src/taglib.o
	$(CC) -o acsi src/main.c src/acsilib.c src/taglib.c
clean:
	rm -f *.o
tclean:
	rm -f i18n-*

default: main clean

test: src/main.o src/acsilib.o src/taglib.o
	$(CC) -o test src/main.c src/acsilib.c src/taglib.c

t: test

tc: tclean

all: main test clean
