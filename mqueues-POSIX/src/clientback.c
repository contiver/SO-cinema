#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <mqueue.h>
#include "mutual.h"
#include "../../common/error_handling.h"
#include "../../common/clientback.h"
#include "../../common/shared.h"
#include "../../common/ipc.h"
 
void communicate(void);

static ReqMsg reqMsg;
static RespMsg respMsg;
static char cltname[100];
static mqd_t qin, qout;

void
initializeClient(void){
    signal(SIGINT, onSigInt);
    reqMsg.mtype = (long) getpid();

    struct mq_attr respAttr;
    respAttr.mq_maxmsg = 10;
    respAttr.mq_msgsize = sizeof(RespMsg);

    struct mq_attr reqAttr;
    reqAttr.mq_maxmsg = 10;
    reqAttr.mq_msgsize = sizeof(ReqMsg);

    sprintf( cltname, CLIENT_NAME_TEMPLATE, (long)getpid() );
    printf("cltname = %s\n", cltname);
    if ( (qin = mq_open(cltname, O_RDONLY|O_CREAT, 0666, &respAttr)) == -1 )
        fatal("Error mq_open qin");
    if ( (qout = mq_open(SERVER_NAME, O_WRONLY|O_CREAT, 0666, &reqAttr)) == -1 )
        fatal("Error mq_open qout");
    return;
}

void
onSigInt(int sig){
    terminateClient();
}
 
void
terminateClient(void){
    int exit_status = EXIT_SUCCESS;
    if( mq_close(qin) == -1) exit_status = EXIT_FAILURE;
    if( mq_close(qout) == -1) exit_status = EXIT_FAILURE; 
    if( mq_unlink(cltname) == -1) exit_status = EXIT_FAILURE;
    exit(exit_status);
}

Movie
get_movie(int movieID){
    reqMsg.req.comm = GET_MOVIE;
    reqMsg.req.movieID = movieID;
    communicate();
    return respMsg.resp.m;
}

Matrix
get_movies_list(void){
    reqMsg.req.comm = MOVIE_LIST;
    communicate();
    return respMsg.resp.matrix;
}

int
cancel_seat(Client c, int movieID, int seat){
    reqMsg.req.comm = CANCEL_SEAT;
    reqMsg.req.movieID = movieID;
    reqMsg.req.seat = seat;
    communicate();
    return respMsg.resp.ret;
}

int
reserve_seat(Client c, int movieID, int seat){
    reqMsg.req.comm = RESERVE_SEAT;
    reqMsg.req.client = c;
    reqMsg.req.movieID = movieID;
    reqMsg.req.seat = seat;
    communicate();
    return respMsg.resp.ret;
}

void
communicate(void){
    mq_send(qout, (char *)&reqMsg, sizeof(ReqMsg), 0);
    mq_receive(qin, (char *)&respMsg, sizeof(RespMsg), NULL);
}
