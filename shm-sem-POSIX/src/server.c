#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include "../../common/ipc.h"
#include "../../common/shared.h"
#include "../../common/dbAccess.h"
#include "mutual.h"

Response execRequest(Request r);
static Request *req;
static Response *resp;

int main(int argc, char *argv[]){

    req = getmem(); // Request is bigger than Response
    printf("dir de memoria: %d\n",(int)req);
    memset(req, 0, SIZE);
    initmutex();
    resp = malloc(sizeof(Response));

    while( 1 ){
        enter2();
        *resp = execRequest(*req);
        memcpy((Response *) req, (Request *) resp, sizeof(Response));
        leave3();
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
        //    printf("%d", r.movieID);// TODO END MY SUFFERING, DELETE ME
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
