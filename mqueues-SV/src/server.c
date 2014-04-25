#include <signal.h>
#include "../../common/dbAccess.h"
#include "mutual.h"

void terminateServer(int sig);
void execRequest(void);

void
fatal(char *s)
{
    perror(s);
    exit(EXIT_FAILURE);
}

static ReqMsg reqMsg;
static RespMsg respMsg;
static int msqid =-1,msqout=-1;
 
int
main(void){
    char cltname[100];
    
    signal(SIGINT, terminateServer);
   
    struct mq_attr reqAttr;
    reqAttr.mq_maxmsg = 10;
    reqAttr.mq_msgsize = sizeof(ReqMsg);
    
    if ( (msqid = msgget(IPC_PRIVATE, IPC_CREAT | 0666) == -1  )
        fatal("Error msgget");
         
    for(;;){
        msgrcv(msqid, (char *)&reqMsg, sizeof(reqMsg), 0, IPC_NOWAIT);
        execRequest(); 
        sprintf(cltname, CLIENT_NAME_TEMPLATE, reqMsg.mtype);
        if ( (msqout = msgget(cltname, O_WRONLY)) == -1 ){
            perror("Error mq_open qout");
            continue;
        }
        msgsend(msqout, (char *)&respMsg, sizeof(RespMsg), IPC_NOWAIT);
        if(mq_close(qout) == -1)
            printf("Error closing qout descriptor. Ignoring...\n");
    }
}

void
terminateServer(int sig){
    int exit_status = EXIT_SUCCESS;
    if( qin != -1 ){
        if( mq_close(qin) == -1 ) exit_status = EXIT_FAILURE;
        if( mq_unlink(SERVER_NAME) == -1 ) exit_status = EXIT_FAILURE;
    }
    if( qout != -1 && mq_close(qout) == -1 ) exit_status = EXIT_FAILURE;
    exit(exit_status);
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
            respMsg.resp.matrix = get_movies_list();
            break;
        default:
            printf("Invalid command number received\n");
            /* avoid other clients to read previous client's response */
            memset(&respMsg, 0, sizeof(respMsg));
            respMsg.resp.ret = -1;
    }
}
