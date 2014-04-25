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
#include "../../common/clientback.h"
#include "../../common/shared.h"
#include "../../common/ipc.h"


static ReqMsg reqMsg;
static RespMsg respMsg;
static int msqin =-1,msqout=-1;
void communicate(void);
 
void
fatal(char *s)
{
    perror(s);
    exit(EXIT_FAILURE);
}

void
onSigInt(int sig){
    terminateClient();
}
 
void
initializeClient(void){
    signal(SIGINT, onSigInt);
    reqMsg.mtype = (long) getpid();

	msqin= msgget(SERVER_KEY, IPC_CREAT | S_IRUSR);
	if(msqin==-1)
		fatal("msgget");

	msqout= msgget(CLIENTS_KEY, IPC_CREAT | S_IWUSR);
	if(msqout==-1)
		fatal("msgget");
	
}

void
terminateClient(void){
	exit(EXIT_SUCCESS);
    //int exit_status = EXIT_SUCCESS;
    //if( mq_close(qin) == -1) exit_status = EXIT_FAILURE;
    //if( mq_close(qout) == -1) exit_status = EXIT_FAILURE; 
   // if( mq_unlink(cltname) == -1) exit_status = EXIT_FAILURE;
 //   exit(exit_status);
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

	if(msgsnd(msqout, (char *)&reqMsg, sizeof(ReqMsg), IPC_NOWAIT)==-1){
		printf("error msgsnd\n");
	}
	if(msgrcv(msqin, (char *)&respMsg , sizeof(respMsg), getpid(), IPC_NOWAIT)==-1){
		printf("error msgrcv\n");
	}
}
