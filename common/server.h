#ifndef SERVER_H
#define SERVER_H

#include "ipc.h"

void terminateServer(void);
Response execRequest(Request request);
void onSigInt(int sig);

#endif
