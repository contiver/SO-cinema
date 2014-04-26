#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"
#include "dbAccess.h"
#include "shared.h"
#include "ipc.h"

void
fatal(char *s){
    perror(s);
    exit(EXIT_FAILURE);
}

Response
execRequest(Request r){
    Response response;

    switch(r.comm){
        case RESERVE_SEAT:
            response.ret = reserve_seat(r.client, r.movieID, r.seat);
            break;
        case CANCEL_SEAT:
            response.ret = cancel_seat(r.client, r.movieID, r.seat);
            break;
        case GET_MOVIE:
            response.m = get_movie(r.movieID);
            break;
        case MOVIE_LIST:
            response.matrix = get_movies_list();
            break;
        default:
            printf("Invalid command number received\n");
            /* Avoid other clients to read previous client's response */
            memset(&response, 0, sizeof(Response));
            response.ret = -1;
    }
    return response;
}

void
onSigInt(int sig){
    terminateServer();
}
