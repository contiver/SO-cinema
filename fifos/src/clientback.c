#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "../../include/clientback.h"

#define FIFO_PATH "./fifo";

static char clientFifo[CLIENT_FIFO_NAME_LEN];
static int serverFd, clientFd;
static Request req;
req.pid = (long)getpid();

void
fatal(char *s){
    perror(s);
    exit(1);
}

static void
removeFifo(void){
    unlink(clientFifo);
}

void initializer(){
    /* Create the client FIFO */
    umask(0); // Ver bien para que es esto
    snprintf(clientFifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE,
                (pid_t) getpid());
    if(mkfifo(clientFifo), S_IRUSR | S_IWUSR | S_IWGRP) == -1
        && errno != EEXIST){
        // errExit ? hacer algo!
    }
    if(atexit(removeFifo) != 0){
        // errExit ? hacer algo!
    }
    clientFd = open(clientFifo, O_RDONLY);
    if(clientFd == -1){
        printf("Error opening client FIFO\n");
        exit(1);
    }

    /* Open the server FIFO to send requests */
    serverFd = open(SERVER_FIFO, O_WRONLY);
    if(serverFd == -1){
        printf("Error opening server FIFO\n");
        exit(1);
    }
}

Movie
get_movie(int movieID){
    req.comm = GET_MOVIE;
    req.movieID = movieID;
    writeInFifo();

    if(read(clientFd, &resp, sizeof(Response)) != sizeof(Response))
        fatal("Can't read response from the server")

    return req.m;
}

void
writeInFifo(){
    if(write(serverFd, &req, sizeof(Request)) != sizeof(Request))
        fatal("Can't write to server");

    clientFd = open(clientFifo, O_RDONLY);
    if(read(clientFd, &resp, sizeof(Response)) != sizeof(Response))
        fatal("Can't read response from the server")
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
        fatal("Can't read response from the server")

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
        fatal("Can't read response from the server")

    return resp.ret;
}
