CC = gcc
CFLAGS = -Wall -O0 -g -ggdb
OBJECTS = pack.o unpack.o
LIBS = 

all: pack unpack

pack: Makefile pack.o
	$(CC) $(CFLAGS) pack.o -o pack $(LIBS)

unpack: Makefile unpack.o
	$(CC) $(CFLAGS) unpack.o -o unpack $(LIBS)

pack.o: Makefile pack.c pack.h
	$(CC) $(CFLAGS) -c pack.c -o pack.o

unpack.o: Makefile unpack.c pack.h
	$(CC) $(CFLAGS) -c unpack.c -o unpack.o

clean:
	rm -f pack unpack $(OBJECTS)
