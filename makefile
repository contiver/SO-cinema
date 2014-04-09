CC = gcc
CFLAGS = -Wall
TARGET = client
VPATH = src:include

default: client

client: client.o backend.o
	$(CC) $(CFLAGS) -o client client.o backend.o 

src/client.o: client.c client.h backend.h
	$(CC) $(CFLAGS) -c src/client.c src/backend.c

backend.o: backend.c backend.h
	$(CC) $(CFLAGS) -c src/backend.c

clean:
	$(RM) client *.o *~
