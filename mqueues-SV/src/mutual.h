#ifndef MUTUAL_H
#define MUTUAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <fcntl.h>
#include <signal.h>
#include <sys/msg.h>
#include <sys/types.h>
#include "../../common/shared.h"
#include "../../common/ipc.h"

#define SERVER_KEY ftok("/mqueues-SV/server", 0)
#define CLIENTS_KEY ftok("/mqueues-SV/clients", 0)

void onSigInt(int sig);

typedef struct {
    long mtype;
    Request req;
} ReqMsg;

typedef struct {
    long mtype;
    Response resp;
} RespMsg;

#endif
