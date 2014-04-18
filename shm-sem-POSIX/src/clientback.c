#include <stdio.h>
#include <string.h>
#include "mutual.h"
#include "../../common/shared.h"
#include "../../common/ipc.h"
 
static Request *req;
static Response *resp;

void
initializeClient(void){
    req = getmem();
    initmutex();
   /*
    while ( (n = read(0, resp, sizeof(Response))) > 0  ){
            enter();
            sprintf(req, "%->*s", n, buf);
            printf("Cliente escribe: %s", req);
            leave();
        }
    printf("Cliente termina\n");
    */
    return;
 }

/*

int
initiateConnection(){
    req->comm = TEST_CONNECTION;
    resp->ret = -1; // if value isn't set to 0 on return an error occurred
    printf("1\n");
    writeInFifo();
    printf("2\n");

    clientFd = open(clientFifo, O_RDONLY);
    if(clientFd == -1){
        printf("Error opening client FIFO\n");
        exit(1);
    }
    printf("3\n");
    if(read(clientFd, &resp, sizeof(Response)) != sizeof(Response)){
        fatal("Can't read response from the server 4\n");
    }
    printf("termine de leer\n");

    return resp->ret;
}
*/

Movie
get_movie(int movieID){
    enter1();
    req->comm = GET_MOVIE;
    req->movieID = movieID;
    leave2();
    enter3();
    resp = (Response *) req;
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
    req->seat = seat; leave2();
    enter3();
    resp = (Response *) req;
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
    resp = (Response *) req;
    leave1();
    return resp->ret;
}
