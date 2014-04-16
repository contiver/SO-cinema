CC = gcc
CFLAGS = -Wall
TARGET = client
SRC = ./src
BIN = ./bin
COMMON = ../include

default: client

client: $(BIN)/clientfront.o dbAccess.o
	$(CC) $(CFLAGS) -o $(BIN)/client clientfront.o dbAccess.o 

$(BIN)/clientfront.o: $(SRC)/clientfront.c $(COMMON)/dbAccess.h $(COMMON)/clientfront.h
	$(CC) $(CFLAGS) -c $(SRC)/clientfront.c $(COMMON)/dbAccess.c

dbAccess.o: $(COMMON)/dbAccess.c $(COMMON)/dbAccess.h
	$(CC) $(CFLAGS) -c $(COMMON)/dbAccess.c

clean:
	$(RM) client *.o *~
