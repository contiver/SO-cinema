CC = gcc
CFLAGS = -Wall
TARGET = client
SRC = ../common
BIN = ./bin
#SRC = ../include

default: client

client: $(BIN)/clientfront.o dbAccess.o clientback.o
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) -o $(BIN)/client clientfront.o dbAccess.o clientback.o 

$(BIN)/clientfront.o: $(SRC)/clientfront.c $(SRC)/dbAccess.h $(SRC)/clientfront.h $(SRC)/clientback.h
	$(CC) $(CFLAGS) -c $(SRC)/clientfront.c $(SRC)/dbAccess.c ./src/clientback.c

dbAccess.o: $(SRC)/dbAccess.c $(SRC)/dbAccess.h
	$(CC) $(CFLAGS) -c $(SRC)/dbAccess.c

clientback.o: ./src/clientback.c $(SRC)/clientback.h
	$(CC) $(CFLAGS) -c ./src/clientback.c 

clean:
	$(RM) client *.o *~
