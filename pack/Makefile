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

clean:
	rm -f pack unpack list $(OBJECTS)
