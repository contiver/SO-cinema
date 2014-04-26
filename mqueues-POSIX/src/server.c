#include <signal.h>
#include "../../common/dbAccess.h"
#include "../../common/server.h"
#include "../../common/error_handling.h"
#include "mutual.h"

static mqd_t qin = -1, qout = -1;
 
int
main(void){
    char cltname[100];
    ReqMsg reqMsg;
    RespMsg respMsg;

    signal(SIGINT, onSigInt);

    struct mq_attr reqAttr;
    reqAttr.mq_maxmsg = 10;
    reqAttr.mq_msgsize = sizeof(ReqMsg);

    if ( (qin = mq_open(SERVER_NAME, O_RDONLY|O_CREAT, 0666, &reqAttr)) == -1  )
        fatal("Error mq_open qin");

    for(;;){
        mq_receive(qin, (char *)&reqMsg, sizeof(reqMsg), NULL);
        respMsg.resp = execRequest(reqMsg.req); 
        sprintf(cltname, CLIENT_NAME_TEMPLATE, reqMsg.mtype);
        if ( (qout = mq_open(cltname, O_WRONLY)) == -1 ){
            perror("Error mq_open qout");
            continue;
        }
        mq_send(qout, (char *)&respMsg, sizeof(RespMsg), 0);
        if(mq_close(qout) == -1)
            printf("Error closing qout descriptor. Ignoring...\n");
    }
}

void
terminateServer(void){
    int exit_status = EXIT_SUCCESS;
    if( qin != -1 ){
        if( mq_close(qin) == -1 ) exit_status = EXIT_FAILURE;
        if( mq_unlink(SERVER_NAME) == -1 ) exit_status = EXIT_FAILURE;
    }
    if( qout != -1 && mq_close(qout) == -1 ) exit_status = EXIT_FAILURE;
    exit(exit_status);
}

void
onSigInt(int sig){
    terminateServer();
}
