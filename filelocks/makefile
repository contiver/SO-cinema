CC = gcc
CFLAGS = -Wall
TARGET = client
SRC = ./src
BIN = ./bin

default: client

client: $(BIN)/client.o backend.o
	$(CC) $(CFLAGS) -o $(BIN)/client client.o backend.o 

$(BIN)/client.o: $(SRC)/client.c ../include/common.h ../include/client.h
	$(CC) $(CFLAGS) -c $(SRC)/client.c $(SRC)/backend.c

backend.o: $(SRC)/backend.c ../include/common.h
	$(CC) $(CFLAGS) -c $(SRC)/backend.c

clean:
	$(RM) client *.o *~
