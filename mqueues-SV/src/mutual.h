#ifndef MUTUAL_H
#define MUTUAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/msg.h>
#include <sys/types.h>
#include "../../common/shared.h"
#include "../../common/ipc.h"

#define SERVER_KEY ftok("/tmp/mqueues-SV-server", 2)
#define CLIENTS_KEY ftok("/tmp/mqueues-SV-clients", 1)

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
