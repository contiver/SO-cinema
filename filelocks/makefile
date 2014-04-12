CC = gcc
CFLAGS = -Wall
TARGET = client
SRC = ./src
BIN = ./bin
INCLUDE = ../include

default: client

client: $(BIN)/clientfront.o clientback.o
	$(CC) $(CFLAGS) -o $(BIN)/client clientfront.o clientback.o 

$(BIN)/clientfront.o: $(SRC)/clientfront.c $(INCLUDE)/clientback.h $(INCLUDE)/clientfront.h
	$(CC) $(CFLAGS) -c $(SRC)/clientfront.c $(SRC)/clientback.c

clientback.o: $(SRC)/clientback.c ../include/clientback.h
	$(CC) $(CFLAGS) -c $(SRC)/clientback.c

clean:
	$(RM) client *.o *~
