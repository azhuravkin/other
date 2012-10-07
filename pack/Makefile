CC = gcc
CFLAGS = -Wall -O0 -g -ggdb
OBJECTS = pack.o unpack.o list.o
LIBS = 

all: pack unpack list

pack: Makefile pack.o
	$(CC) $(CFLAGS) pack.o -o pack $(LIBS)

unpack: Makefile unpack.o
	$(CC) $(CFLAGS) unpack.o -o unpack $(LIBS)

list: Makefile list.o
	$(CC) $(CFLAGS) list.o -o list $(LIBS)

list.o: Makefile list.c pack.h
	$(CC) $(CFLAGS) -c list.c -o list.o

unpack.o: Makefile unpack.c pack.h
	$(CC) $(CFLAGS) -c unpack.c -o unpack.o

clean:
	rm -f pack unpack list $(OBJECTS)
