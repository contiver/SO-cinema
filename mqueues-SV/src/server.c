#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include "../../common/dbAccess.h"
#include "../../common/server.h"
#include "../../common/error_handling.h"
#include "mutual.h"

static int msqin = -1, msqout = -1;

int
main(void){
    ReqMsg reqMsg;
    RespMsg respMsg;
    signal(SIGINT, onSigInt);

    FILE *f1 = fopen("/tmp/mqueues-SV-clients","wb");
    if( f1 == NULL )
        fatal("fopen");
    fclose(f1);
    
    FILE *f2 = fopen("/tmp/mqueues-SV-server","wb");
    if( f2 == NULL )
        fatal("fopen");
    fclose(f2);

    if( (msqin = msgget(CLIENTS_KEY, IPC_CREAT | 0666)) == -1 )
        fatal("msgget");
    if ( (msqout = msgget(SERVER_KEY, IPC_CREAT | 0666)) == -1 )
        fatal("msgget");

    for(;;){
        if( msgrcv(msqin, (char *)&reqMsg, sizeof(reqMsg), 0, 0) == -1 ){
            if(errno == EINTR)
                continue;
            perror("msgrcv");
            break;
        }
        respMsg.resp = execRequest(reqMsg.req);
        respMsg.mtype = reqMsg.mtype;
        if( msgsnd(msqout, (char *)&respMsg, sizeof(RespMsg), 0) == -1 ){
            perror("msgsnd");
            break;
        }
    }
    terminateServer();
    return 0;
}

void
terminateServer(void){
    int exit_status = EXIT_SUCCESS;
    if( msqin != -1 && msgctl(msqin, IPC_RMID, NULL) == -1 ) exit_status = EXIT_FAILURE;
    if( msqout != -1 && msgctl(msqout, IPC_RMID, NULL) == -1 ) exit_status = EXIT_FAILURE;
    if( remove("/tmp/mqueues-SV-clients") == -1 ) exit_status = EXIT_FAILURE;  
    if( remove("/tmp/mqueues-SV-server") == -1 ) exit_status = EXIT_FAILURE;  
    exit(exit_status);
}

void
onSigInt(int sig){
    terminateServer();
}
