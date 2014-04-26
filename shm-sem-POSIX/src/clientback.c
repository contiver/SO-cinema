#include <stdio.h>
#include <string.h>
#include <signal.h>
#include "mutual.h"
#include "../../common/clientback.h"
#include "../../common/shared.h"
#include "../../common/ipc.h"
 
static Request *req;
static Response *resp;

void
initializeClient(void){
    req = (Request *)getmem();
    resp = malloc(sizeof(Response));
    initmutex();
    signal(SIGINT, onSigInt);
    return;
}

void
onSigInt(int sig){
    terminateClient();
}

Movie
get_movie(int movieID){
    enter(1);
    req->comm = GET_MOVIE;
    req->movieID = movieID;
    leave(2);
    enter(3);
    memcpy(resp, req, sizeof(Response));
    leave(1);
    return resp->m;
}

Matrix
get_movies_list(void){
    enter(1);
    req->comm = MOVIE_LIST;
    leave(2);
    enter(3);
    memcpy(resp, req, sizeof(Response));
    leave(1);
return resp->matrix;
}

int
cancel_seat(Client c, int movieID, int seat){
    enter(1);
    req->comm = CANCEL_SEAT;
    req->movieID = movieID;
    req->seat = seat;
    leave(2);
    enter(3);
    memcpy(resp, req, sizeof(Response));
    leave(1);
    return resp->ret;
}

int
reserve_seat(Client c, int movieID, int seat){
    enter(1);
    req->comm = RESERVE_SEAT;
    req->client = c;
    req->movieID = movieID;
    req->seat = seat;
    leave(2);
    enter(3);
    memcpy(resp, req, sizeof(Response));
    leave(1);
    return resp->ret;
}
