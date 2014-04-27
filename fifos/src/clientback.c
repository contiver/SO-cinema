#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../../common/clientback.h"
#include "../../common/dbAccess.h"
#include "../../common/error_handling.h"
#include "../../common/ipc.h"
#include "../../common/shared.h"
#include "../include/fifo.h"

void communicate(void);
static void removeFifo(void);
void onSigInt(int sig);

static char clientFifo[CLIENT_FIFO_NAME_LEN];
static int serverFd = -1, clientFd = -1;
static Request req;
static Response resp;

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
        exit(EXIT_FAILURE);
    }

   /* Open the server FIFO to send requests */
    serverFd = open(SERVER_FIFO, O_WRONLY);
    if(serverFd == -1){
        printf("Error opening server FIFO\n");
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, onSigInt);
    
    if(atexit(removeFifo) != 0){
        printf("atexit error\n");
        exit(EXIT_FAILURE);
    }
}

void
terminateClient(void){
    int exit_status = EXIT_SUCCESS;
    if(close(clientFd)) exit_status = EXIT_FAILURE;
    if(close(serverFd)) exit_status = EXIT_FAILURE;
    if(unlink(clientFifo)) exit_status = EXIT_FAILURE;
    exit(exit_status);
}

void onSigInt(int sig){
    terminateClient();
}

void
communicate(void){
    if( write(serverFd, &req, sizeof(Request)) != sizeof(Request) )
        fatal("Can't write to server\n");

    if( (clientFd = open(clientFifo, O_RDONLY)) == -1){
        printf("Error opening client FIFO\n");
        exit(EXIT_FAILURE);
    }

    if( read(clientFd, &resp, sizeof(Response)) != sizeof(Response) )
        fatal("Can't read response from the server\n");

    if( close(clientFd) == -1 ){
        printf("Error closing the client's FIFO\n");
        exit(EXIT_FAILURE);
    }
}

Movie
get_movie(int movieID){
    req.comm = GET_MOVIE;
    req.movieID = movieID;
    communicate();
    return resp.m;
}

Matrix
get_movies_list(void){
    req.comm = MOVIE_LIST;
    communicate();
	return resp.matrix;
}

int
cancel_seat(Client c, int movieID, int seat){
    req.comm = CANCEL_SEAT;
    req.movieID = movieID;
    req.seat = seat;
    communicate();
    return resp.ret;
}

int
reserve_seat(Client c, int movieID, int seat){
    req.comm = RESERVE_SEAT;
    req.client = c;
    req.movieID = movieID;
    req.seat = seat;
    communicate();
    return resp.ret;
}
