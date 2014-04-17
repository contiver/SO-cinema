#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include "../../common/dbAccess.h"
#include "../../common/shared.h"
#include "../include/fifo.h"
#include "clientback.h"
#include "../../common/initializer.h"

static char clientFifo[CLIENT_FIFO_NAME_LEN];
static int serverFd, clientFd;
static Request req;
static Response resp;

int initiateConnection(void);

void
fatal(char *s){
    perror(s);
    exit(1);
}

static void
removeFifo(void){
    unlink(clientFifo);
}

void
initializeClient(){
    req.pid = (long)getpid();
    
    /* Create the client FIFO */
    umask(0);
    snprintf(clientFifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE,
                (long) getpid());
    if(mkfifo(clientFifo, S_IRUSR | S_IWUSR | S_IWGRP) == -1
        && errno != EEXIST){
        printf("error while creating the fifo\n");
        exit(1);
    }

   /* Open the server FIFO to send requests */
    serverFd = open(SERVER_FIFO, O_WRONLY);
    if(serverFd == -1){
        printf("Error opening server FIFO\n");
        exit(1);
    }
    
    if(initiateConnection() != 0){
        printf("Error stablishing a connection to the server\n");
        exit(1);
    }
    
    printf("test\n");
    if(atexit(removeFifo) != 0){
        printf("atexit error\n");
        exit(1);
    }
    printf("sali de la funcion!\n");
}

int
initiateConnection(){
    req.comm = TEST_CONNECTION;
    resp.ret = -1; /* if value isn't set to 0 on return an error occurred */
    printf("1\n");
    writeInFifo();
    printf("2\n");

    clientFd = open(clientFifo, O_RDONLY);
    if(clientFd == -1){
        printf("Error opening client FIFO\n");
        exit(1);
    }
    printf("3\n");
    if(read(clientFd, &resp, sizeof(Response)) != sizeof(Response)){
        fatal("Can't read response from the server 4\n");
    }
    printf("termine de leer\n");

    return resp.ret;
}

Movie
get_movie(int movieID){
    req.comm = GET_MOVIE;
    req.movieID = movieID;
    writeInFifo();

    if(read(clientFd, &resp, sizeof(Response)) != sizeof(Response))
        fatal("Can't read response from the server\n");

    return resp.m;
}

void
writeInFifo(){
    if(write(serverFd, &req, sizeof(Request)) != sizeof(Request))
        fatal("Can't write to server");

    //clientFd = open(clientFifo, O_RDONLY);
    //if(read(clientFd, &resp, sizeof(Response)) != sizeof(Response))
      //  fatal("Can't read response from the server\n");
}

int
get_movies_list(char *movies[10][60]){
	return 0; // IMPLEMENTAR !!
}

int
cancel_seat(Client c, int movieID, int seat){
    req.comm = CANCEL_SEAT;
    req.movieID = movieID;
    req.seat = seat;
    writeInFifo();

    if(read(clientFd, &resp, sizeof(Response)) != sizeof(Response))
        fatal("Can't read response from the server");

    return resp.ret;
}

int
reserve_seat(Client c, int movieID, int seat){
    req.comm = RESERVE_SEAT;
    req.client = c;
    req.movieID = movieID;
    req.seat = seat;
    writeInFifo();

    if(read(clientFd, &resp, sizeof(Response)) != sizeof(Response))
        fatal("Can't read response from the server");

    return resp.ret;
}
