#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>
#include <netdb.h>
#include "mutual.h"
#include "../../common/ipc.h"
#include "../../common/dbAccess.h"
#include "../../common/shared.h"
#include "../../common/clientback.h"

void communicate(void);
void fatal(char *s);
void onSigInt(int sig);

static int cfd = -1;
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
    struct addrinfo hints, *result, *rp;
    /* Call getaddrinfo() to obtain a list of addresses that
       we can try connecting to */

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_family = AF_UNSPEC; /* Allows IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICSERV;
    if (getaddrinfo("localhost", PORT_NUM, &hints, &result) != 0)
        fatal("getaddrinfo");

    /* Walk through returned list until we find an address structure
       that can be used to successfully connect a socket */

    for(rp = result; rp != NULL; rp = rp->ai_next) {
        cfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (cfd == -1)
            continue; /* On error, try next address */
        if (connect(cfd, rp->ai_addr, rp->ai_addrlen) != -1)
            break; /* Success */
        /* Connect failed: close this socket and try next address */ 
        close(cfd);
    }
    if (rp == NULL)
        fatal("Could not connect socket to any address");
    freeaddrinfo(result);

    if(write(cfd, &req, sizeof(Request)) != sizeof(Request))
        fatal("Can't write to server\n");

    if(read(cfd, &resp, sizeof(Response)) != sizeof(Response))
        fatal("Can't read response from the server\n");

    if(close(cfd) == -1){
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
