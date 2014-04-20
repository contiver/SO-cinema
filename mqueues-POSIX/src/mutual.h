#ifndef MUTUAL_H
#define MUTUAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <mqueue.h>
#include "../../common/shared.h"
#include "../../common/ipc.h"

#define SERVER_NAME "/serv_queue"
#define CLIENT_NAME_TEMPLATE "/client_%ld_queue"

typedef struct {
    long mtype;
    Request req;
} ReqMsg;

typedef struct {
    long mtype;
    Response resp;
} RespMsg;

#endif
