CC=gcc
CFLAGS=-I.
DEPS = ipc.h

#%.o: %.c $(DEPS)
	#$(CC) -c -o $@ $< $(CFLAGS)

all: dir binary

dir:
	mkdir -pv bin

binary: server client

server:
	$(CC) server.c -o bin/server

client:
	$(CC) client.c -o bin/client
