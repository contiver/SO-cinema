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
    // Start with semaphores 2 and 3 in 0 (down)
    enter2();
    enter3();

    for(;;){
        enter2();
        execRequest(*req);
        leave3();
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
            //TODO IMPLEMENTAR
            break;
        case TEST_CONNECTION:
            resp->ret = 0;
            break;
    }
}
