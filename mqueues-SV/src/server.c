#include <signal.h>
#include "../../common/dbAccess.h"
#include "../../common/server.h"
#include "mutual.h"

static int msqin = -1, msqout = -1;

int
main(void){
    ReqMsg reqMsg;
    RespMsg respMsg;
    signal(SIGINT, onSigInt);

    msqin = msgget(CLIENTS_KEY, IPC_CREAT | 0666); 
    if(msqin == -1)
        fatal("msgget");

    msqout = msgget(SERVER_KEY, IPC_CREAT | 0666);
    if(msqout == -1)
        fatal("msgget");

    for(;;){
        if( msgrcv(msqin, (char *)&reqMsg, sizeof(reqMsg), 0, 0) == -1 )
            printf("error msgrcv\n");
        respMsg.resp = execRequest(reqMsg.req);
        respMsg.mtype = reqMsg.mtype;
        if( msgsnd(msqout, (char *)&respMsg, sizeof(RespMsg), 0) == -1 )
            printf("error msgsnd\n");
    }
}

void
terminateServer(void){
    int exit_status = EXIT_SUCCESS;
    if( msqin != -1 && msgctl(msqin, IPC_RMID, NULL) == -1 ) exit_status = EXIT_FAILURE;
    if( msqout != -1 && msgctl(msqout, IPC_RMID, NULL) == -1 ) exit_status = EXIT_FAILURE;
    exit(exit_status);
}
