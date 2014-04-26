#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include "../include/fifo.h"
#include "../../common/shared.h"
#include "../../common/server.h"
#include "../../common/dbAccess.h"
#include "../../common/ipc.h"

static int serverFd = -1, dummyFd = -1, clientFd = -1;

int main(int argc, char *argv[]){
    Request req;
    Response resp; 
    char clientFifo[CLIENT_FIFO_NAME_LEN];

    if(mkfifo(SERVER_FIFO, S_IRUSR | S_IWUSR | S_IWGRP) == -1
            && errno != EEXIST){
        printf("error creating server fifo");
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, onSigInt);

    serverFd = open(SERVER_FIFO, O_RDONLY);
    if(serverFd == -1){
        printf("error opening the server fifo\n");
        exit(EXIT_FAILURE);
    }

    /* Open an extra write descriptor, so that we never see EOF */
    dummyFd = open(SERVER_FIFO, O_WRONLY);
    if(dummyFd == -1){
        printf("error opening dummy fifo\n");
        exit(EXIT_FAILURE);
    }

    /* Ignore the SIGPIPE signal, so that if the server attempts to write to a 
     * client FIFO that doesn't have a reader, then rather than being sent a 
     * SIGPIPE signal which kills it by default, it receives an EPIPE error
     * from the write() syscall */
    if(signal(SIGPIPE, SIG_IGN) == SIG_ERR){
        exit(EXIT_FAILURE);
    }

    for(;;){
        if(read(serverFd, &req, sizeof(Request)) != sizeof(Request)){
            fprintf(stderr, "Error reading request. Discarding...\n");    
            continue;
        } 

        snprintf(clientFifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE,
                (long) req.pid);
        clientFd = open(clientFifo, O_WRONLY);
        if(clientFd == -1){
            printf("couldn't open %s", clientFifo);
            continue;
        }

        /*Send responde and close FIFO */
        resp = execRequest(req);
        if( write(clientFd, &resp, sizeof(Response)) != sizeof(Response) )
            printf("Error writing to FIFO %s", clientFifo);
        if(close(clientFd) == -1){
            printf("error closing FIFO %s\n", clientFifo); 
        }
    }
}

void
onSigInt(int sig){
    terminateServer();
}

void
terminateServer(void){
    int exit_status = EXIT_SUCCESS;
    if(clientFd != -1 && close(clientFd) ) exit_status = EXIT_FAILURE;
    if(dummyFd != -1 && close(dummyFd) ) exit_status = EXIT_FAILURE;
    if(serverFd != -1 && close(serverFd) ) exit_status = EXIT_FAILURE;
    if(unlink(SERVER_FIFO) ) exit_status = EXIT_FAILURE;
    exit(exit_status);
}
