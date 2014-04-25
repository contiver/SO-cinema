#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>
#include "mutual.h"
#include "../../common/ipc.h"
#include "../../common/dbAccess.h"
#include "../../common/shared.h"
#include "../../common/clientback.h"

void communicate(void);
void fatal(char *s);
void onSigInt(int sig);

static int sfd = -1;
static struct sockaddr_un addr;
static Request req;
static Response resp;

void
fatal(char *s){
    perror(s);
    exit(EXIT_FAILURE);
}

void
initializeClient(){
    signal(SIGINT, onSigInt);
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SV_SOCKET_PATH, sizeof(addr.sun_path) -1);
    return;
}

void
terminateClient(void){
    int exit_status = EXIT_SUCCESS;
    exit(exit_status);
}

void onSigInt(int sig){
    terminateClient();
}

Movie
get_movie(int movieID){
    req.comm = GET_MOVIE;
    req.movieID = movieID;
    communicate();
    return resp.m;
}

void
communicate(void){
    if( (sfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
        fatal("socket");

    if(connect(sfd, (struct sockaddr *) &addr,
            sizeof(struct sockaddr_un)) == - 1)
                fatal("connect");
    
    if(write(sfd, &req, sizeof(Request)) != sizeof(Request))
        fatal("Can't write to server\n");

    if(read(sfd, &resp, sizeof(Response)) != sizeof(Response))
        fatal("Can't read response from the server\n");

    if(close(sfd) == -1){
        printf("Error closing the client's FIFO\n");
        exit(EXIT_FAILURE);
    }
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
