CC = gcc
CFLAGS = -Wall
TARGET = client
SRC= ./src
BIN= ./bin

default: db

db: $(BIN)/db.o
	$(CC) $(CFLAGS) -o $(BIN)/db db.o

$(BIN)/db.o:
	$(CC) $(CFLAGS) -c $(SRC)/db.c 

clean:
	$(RM) db *.o *~
