CC = gcc
CFLAGS = -Wall -std=c11
TARGET = client
SRC = ./src
BIN = ./bin

default: db
	
db: db.o
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) db.o -o $(BIN)/db

db.o: $(SRC)/db.c
	$(CC) $(CFLAGS) -c $(SRC)/db.c 

clean:
	$(RM) db *.o *~
