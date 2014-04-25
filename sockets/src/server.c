#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include "mutual.h"
#include "../../common/shared.h"
#include "../../common/dbAccess.h"
#include "../../common/ipc.h"

#define BACKLOG 5

void onSigInt(int sig);
Response execRequest(Request r);

Request req;
Response resp; 
static int sfd = -1, cfd = -1;

void
fatal(char *s){
    perror(s);
    exit(EXIT_FAILURE);
}

int
main(int argc, char *argv[]){
    struct sockaddr_un addr;

    if( (sfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1 )
        fatal("socket");

    signal(SIGINT, onSigInt);

    if(remove(SV_SOCKET_PATH) == -1 && errno != ENOENT)
        //errExit("remove-%s", SV_SOCKET_PATH);
        
    memset(&addr, 0, sizeof(struct sockaddr_un)); /* Clear structure */
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SV_SOCKET_PATH, sizeof(addr.sun_path) - 1);

    if( bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1 )
        fatal("bind");
    if( listen(sfd, BACKLOG) == -1 ) 
        fatal("listen");

    for(;;){
        if( (cfd = accept(sfd, NULL, NULL)) == -1 ){
            perror("accept");        
            continue; // Ver si esto esta bien!
        }

        if( read(cfd, &req, sizeof(Request)) != sizeof(Request) )
            perror("read");
        resp = execRequest(req);

        if( write(cfd, &resp, sizeof(Response)) != sizeof(Response) )
            perror("Couldn't write response to cfd");

        if(close(cfd) == -1){
            perror("Couldn't close cfd. Ignoring...");
        }
    }
}

void
onSigInt(int sig){
    int exit_status = EXIT_SUCCESS;
    if( cfd != -1 && close(cfd) != 0 ) exit_status = EXIT_FAILURE;
    if( sfd != -1 && close(sfd) != 0 ) exit_status = EXIT_FAILURE;
    if( remove(SV_SOCKET_PATH) != 0 ) exit_status = EXIT_FAILURE;
    exit(exit_status);
}

Response
execRequest(Request r){
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
            resp.matrix = get_movies_list();
            break;
        case TEST_CONNECTION:
            resp.ret = 0;
            break;
    }
    return resp;
}
