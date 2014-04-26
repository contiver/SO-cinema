#ifndef CLIENTBACK_H
#define CLIENTBACK_H

#include "dbAccess.h"

/* Used to initialize each implementation's particular requirements */
void initializeClient(void);

/* Use to cleanly terminate a client */
void terminateClient(void);

#endif
