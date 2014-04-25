#include <signal.h>
#include "../../common/dbAccess.h"
#include "mutual.h"

void terminateServer(int sig);
void execRequest(void);

static ReqMsg reqMsg;
static RespMsg respMsg;
static int msqin =-1,msqout=-1;
void
fatal(char *s)
{
    perror(s);
    exit(EXIT_FAILURE);
}
 
int
main(void){
	signal(SIGINT, terminateServer);

	msqin= msgget(CLIENTS_KEY, IPC_CREAT | 0666);
	if(msqin==-1)
		fatal("msgget");

	msqout= msgget(SERVER_KEY, IPC_CREAT | 0666);
	if(msqout==-1)
		fatal("msgget");
	
	for(;;){
		if(msgrcv(msqin, (char *)&reqMsg , sizeof(reqMsg), 0, IPC_NOWAIT)==-1){
		printf("error msgrcv\n");
		}
		execRequest();
		respMsg.mtype=reqMsg.mtype;
		if(msgsnd(msqout, (char *)&respMsg, sizeof(RespMsg), IPC_NOWAIT)==-1){
		printf("error msgsnd\n");
		}
	}
}


void
terminateServer(int sig){
    int exit_status = EXIT_SUCCESS;
    if( msqin != -1 && msgctl(msqin,IPC_RMID,NULL) == -1 ) exit_status = EXIT_FAILURE;
    if( msqout != -1 && msgctl(msqout,IPC_RMID,NULL) == -1 ) exit_status = EXIT_FAILURE;
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
