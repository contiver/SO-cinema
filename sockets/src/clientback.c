#include <arpa/inet.h>
#include <errno.h>
#include "mutual.h"
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include "rdwrn.h"
#include "../../common/clientback.h"
#include "../../common/error_handling.h"
#include "../../common/dbAccess.h"
#include "../../common/ipc.h"
#include "../../common/shared.h"

#define HARDCODED_SV_IP "192.168.1.100"

void communicate(void);
void fatal(char *s);
void onSigInt(int sig);

static int cfd = -1;
static Request req;
static Response resp;
static char server_ip[14];

void
initializeClient(){
    printf("Insert server IP to connect: ");
    fflush(stdout);
    scanf("%s", server_ip);
    signal(SIGINT, onSigInt);
}

void
terminateClient(void){
    exit(EXIT_SUCCESS);
}

void onSigInt(int sig){
    terminateClient();
}

void
communicate(void){
    struct sockaddr_in addr;
    /* Construct server address, and make the connection */
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT_NUM);
    inet_pton(AF_INET, server_ip, &(addr.sin_addr));
    if( (cfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        fatal("socket");
    if(connect(cfd, (struct sockaddr *) &addr, 
                sizeof(struct sockaddr_in)) == -1)
        fatal("connect");

    if(writen(cfd, &req, sizeof(Request)) != sizeof(Request))
        fatal("Can't write to server\n");

    if(readn(cfd, &resp, sizeof(Response)) != sizeof(Response))
        fatal("Can't read response from the server\n");

    if(close(cfd) == -1){
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
