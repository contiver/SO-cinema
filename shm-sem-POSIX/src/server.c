#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>
#include "../../common/ipc.h"
#include "../../common/shared.h"
#include "../../common/dbAccess.h"
#include "mutual.h"

void execRequest(Request r);
static Request *req;
static Response *resp;

int
main(int argc, char *argv[]){
    req = (Request *)getmem(); // Request is bigger than Response
    memset(req, 0, SIZE);
    // Interpret same memory area as a Response rather than a Request
    resp = (Response *) req; 

    initmutex();
    // Start with semaphores 2 and 3 in 0 (down)
    enter2();
    enter3();

    while( true ){
        enter2();
        execRequest(*req);
        leave3();
    }
}

void
execRequest(Request r){
    switch(r.comm){
        case RESERVE_SEAT:
            resp->ret = reserve_seat(r.client, r.movieID, r.seat);
            break;
        case CANCEL_SEAT:
            resp->ret = cancel_seat(r.client, r.movieID, r.seat);
            break;
        case GET_MOVIE:
            resp->m = get_movie(r.movieID);
            break;
        case MOVIE_LIST:
            //TODO IMPLEMENTAR
            break;
        case TEST_CONNECTION:
            resp->ret = 0;
            break;
    }
}
