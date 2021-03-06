CC = gcc
CFLAGS = -Wall
TARGET = client
SRC = ./src
BIN = ./bin
COMMON = ../common

default: client server

client: clientfront.o clientback.o mutual.o error_handling.o
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) -o $(BIN)/client clientfront.o clientback.o mutual.o error_handling.o -lpthread -lrt 

clientfront.o: $(COMMON)/clientfront.c $(COMMON)/clientfront.h $(COMMON)/dbAccess.h $(COMMON)/shared.h 
	$(CC) $(CFLAGS) -c $(COMMON)/clientfront.c 

clientback.o: $(SRC)/clientback.c $(COMMON)/ipc.h $(COMMON)/shared.h $(COMMON)/dbAccess.h $(SRC)/mutual.c $(SRC)/mutual.h
	$(CC) $(CFLAGS) -c $(SRC)/clientback.c $(SRC)/mutual.c -lrt

mutual.o: $(SRC)/mutual.c $(SRC)/mutual.h $(COMMON)/error_handling.h $(COMMON)/error_handling.c
	$(CC) $(CFLAGS) -lrt $(SRC)/mutual.c $(COMMON)/error_handling.c

error_handling.o: $(COMMON)/error_handling.c $(COMMON)/error_handling.h 
	$(CC) $(CFLAGS) -c $(COMMON)/error_handling.c

server: server.o $(COMMON)/dbAccess.o mutual.o error_handling.o servercommon.o
	$(CC) $(CFLAGS) -o $(BIN)/server server.o $(COMMON)/dbAccess.o mutual.o error_handling.o servercommon.o -lpthread -lrt

servercommon.o: $(COMMON)/servercommon.o $(COMMON)/server.h
	$(CC) $(CFLAGS) -c $(COMMON)/servercommon.c

server.o: $(SRC)/server.c $(COMMON)/dbAccess.c $(SRC)/mutual.c $(COMMON)/ipc.h $(COMMON)/shared.h $(COMMON)/servercommon.c
	$(CC) $(CFLAGS) -c $(SRC)/server.c $(COMMON)/dbAccess.c $(SRC)/mutual.c $(COMMON)/servercommon.c

dbAccess.o: $(COMMON)/dbAccess.c $(COMMON)/dbAccess.h $(COMMON)/shared.h 
	$(CC) $(CFLAGS) -c $(COMMON)/dbAccess.c

clean:
	$(RM) client *.o *~
