CC = gcc
Cflags = -Wall

SRCS = index.c index.h

all:
	$(CC) $(Cflags) $(SRCS) -o index

debug:
	$(CC) $(Cflags) $(SRCS) -g -o debug

clean:
	rm -f *.o index debug
