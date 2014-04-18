CC = gcc
CFLAGS = -Wall
TARGET = client
SRC = ../common
BIN = ./bin
#SRC = ../include

default: client

client: $(BIN)/clientfront.o dbAccess.o initializer.o
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) -o $(BIN)/client clientfront.o dbAccess.o initializer.o 

$(BIN)/clientfront.o: $(SRC)/clientfront.c $(SRC)/dbAccess.h $(SRC)/clientfront.h $(SRC)/initializer.h
	$(CC) $(CFLAGS) -c $(SRC)/clientfront.c $(SRC)/dbAccess.c ./src/initializer.c

dbAccess.o: $(SRC)/dbAccess.c $(SRC)/dbAccess.h
	$(CC) $(CFLAGS) -c $(SRC)/dbAccess.c

initializer.o: ./src/initializer.c $(SRC)/initializer.h
	$(CC) $(CFLAGS) -c ./src/initializer.c 

clean:
	$(RM) client *.o *~
