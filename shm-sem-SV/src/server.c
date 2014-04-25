#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include "../../common/ipc.h"
#include "../../common/shared.h"
#include "../../common/dbAccess.h"
#include "mutual.h"

void execRequest(Request r);
static Request *req;
static Response *resp;

int
main(void){
    req = (Request *)getmem();
    memset(req, 0, SIZE);
    resp = (Response *)req; 

    initmutex();
    signal(SIGINT, onSigInt);

    for(;;){
        enter(2);
        execRequest(*req);
        leave(3);
    }
    return 0;
}

void
onSigInt(int sig){
    terminateServer();
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
            resp->matrix = get_movies_list(); 
            break;
        case TEST_CONNECTION:
            resp->ret = 0;
            break;
    }
}
