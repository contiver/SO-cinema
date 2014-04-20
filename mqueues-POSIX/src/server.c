#include <signal.h>
#include "../../common/dbAccess.h"
#include "mutual.h"

void terminateServer(int sig);

void
fatal(char *s)
{
    perror(s);
    exit(1);
}

static ReqMsg reqMsg;
static RespMsg respMsg;
 
int
main(void){
    mqd_t qin, qout;
    int n;
    char cltname[100];
    
    signal(SIGINT, terminateServer);
    struct mq_attr reqAttr;
    reqAttr.mq_maxmsg = 10;
    reqAttr.mq_msgsize = sizeof(ReqMsg);
   
    if ( (qin = mq_open(SERVER_NAME, O_RDONLY|O_CREAT, 0666, &reqAttr)) == -1  )
        fatal("Error mq_open qin");
         
    while ( true ){
        mq_receive(qin, (char *)&reqMsg, sizeof(reqMsg), NULL);
        execRequest(); 
        sprintf(cltname, CLIENT_NAME_TEMPLATE, reqMsg.mtype);
        if ( (qout = mq_open(cltname, O_WRONLY)) == -1  )
            fatal("Error mq_open qout");
        mq_send(qout, (char *)&respMsg, sizeof(RespMsg), 0);
    }
}

void
terminateServer(int sig){
    mq_unlink(SERVER_NAME);
    exit(EXIT_SUCCESS);
}

void
execRequest(void){
    Request r = reqMsg.req;
    switch(r.comm){
        case RESERVE_SEAT:
            respMsg.resp.ret = reserve_seat(r.client, r.movieID, r.seat);
            break;
        case CANCEL_SEAT:
            respMsg.resp.ret = cancel_seat(r.client, r.movieID, r.seat);
            break;
        case GET_MOVIE:
            respMsg.resp.m = get_movie(r.movieID);
            break;
        case MOVIE_LIST:
            //TODO IMPLEMENTAR
            break;
        case TEST_CONNECTION:
            respMsg.resp.ret= 0;
            break;
    }
}
