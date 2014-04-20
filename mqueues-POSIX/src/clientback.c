#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <mqueue.h>
#include "mutual.h"
#include "../../common/clientback.h"
#include "../../common/shared.h"
#include "../../common/ipc.h"
 
static ReqMsg reqMsg;
static RespMsg respMsg;
static char cltname[100];
static mqd_t qin, qout;

void onSigInt(int sig);
 
void
fatal(char *s)
{
    perror(s);
    exit(1);
}

void
onSigInt(int sig){
    terminateClient();
}
 
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
    if ( (qin = mq_open(cltname, O_RDONLY|O_CREAT, 0666, &respAttr)) == -1  )
        fatal("Error mq_open qin");
    if ( (qout = mq_open(SERVER_NAME, O_WRONLY|O_CREAT, 0666, &reqAttr)) == -1  )
        fatal("Error mq_open qout");
    return;
}

void
terminateClient(void){
    mq_close(qin);
    mq_close(qout);
    mq_unlink(cltname);
    exit(EXIT_SUCCESS);
}

Movie
get_movie(int movieID){
    printf("El mtype dentro de getmovie es: %ld\n", reqMsg.mtype);
    reqMsg.req.comm = GET_MOVIE;
    reqMsg.req.movieID = movieID;
    mq_send(qout, &reqMsg, sizeof(ReqMsg), 0);
    mq_receive(qin, &respMsg, sizeof(RespMsg), NULL);
    return respMsg.resp.m;
}

int
get_movies_list(char *movies[10][60]){
    return 0; // IMPLEMENTAR !!
}

int
cancel_seat(Client c, int movieID, int seat){
    reqMsg.req.comm = CANCEL_SEAT;
    reqMsg.req.movieID = movieID;
    reqMsg.req.seat = seat;
    mq_send(qout, &reqMsg, sizeof(ReqMsg), 0);
    mq_receive(qin, &respMsg, sizeof(RespMsg), NULL);
    return respMsg.resp.ret;
}

int
reserve_seat(Client c, int movieID, int seat){
    reqMsg.req.comm = RESERVE_SEAT;
    reqMsg.req.client = c;
    reqMsg.req.movieID = movieID;
    reqMsg.req.seat = seat;
    mq_send(qout, &reqMsg, sizeof(ReqMsg), 0);
    mq_receive(qin, &respMsg, sizeof(RespMsg), NULL);
    return respMsg.resp.ret;
}
