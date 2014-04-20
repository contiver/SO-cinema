#include <stdio.h>
#include <string.h>
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
    return;
}

void
terminateClient(void){
    terminate();
}

Movie
get_movie(int movieID){
    enter1();
    req->comm = GET_MOVIE;
    req->movieID = movieID;
    leave2();
    enter3();
    memcpy(resp, req, sizeof(Response));
    leave1();
    return resp->m;
}

int
get_movies_list(char *movies[10][60]){
	return 0; // IMPLEMENTAR !!
}

int
cancel_seat(Client c, int movieID, int seat){
    enter1();
    req->comm = CANCEL_SEAT;
    req->movieID = movieID;
    req->seat = seat;
    leave2();
    enter3();
    memcpy(resp, req, sizeof(Response));
    leave1();
    return resp->ret;
}

int
reserve_seat(Client c, int movieID, int seat){
    enter1();
    req->comm = RESERVE_SEAT;
    req->client = c;
    req->movieID = movieID;
    req->seat = seat;
    leave2();
    enter3();
    memcpy(resp, req, sizeof(Response));
    leave1();
    return resp->ret;
}
