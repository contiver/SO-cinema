#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include "../include/fifo.h"
#include "../../common/shared.h"
#include "../../common/dbAccess.h"
#include "mutual.h"

Response execRequest(Request r);
static Request *req;
static Response *resp;

int main(int argc, char *argv[]){

    req = getmem(); // Request is bigger than Response
    initmutex();
    resp = malloc(sizeof(Response));

    while( 1 ){
        sem_wait(s2);
        *resp = execRequest(*req);
        memcpy((Response *) resp, (Request *) req, sizeof(Response));
        sem_post(s3);
    }
}

Response execRequest(Request r){
    Response resp;

    switch(r.comm){
        case RESERVE_SEAT:
            resp.ret = reserve_seat(r.client, r.movieID, r.seat);
            break;
        case CANCEL_SEAT:
            resp.ret = cancel_seat(r.client, r.movieID, r.seat);
            break;
        case GET_MOVIE:
            resp.m = get_movie(r.movieID);
            break;
        case MOVIE_LIST:
            //TODO IMPLEMENTAR
            break;
        case TEST_CONNECTION:
            resp.ret = 0;
            break;
    }
    return resp;
}
