#ifndef MUTUAL_H
#define MUTUAL_H

#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include "../../common/ipc.h"

#define SHM_SIZE sizeof(Request) /* A request is bigger than a response */
 
void *getmem(void);
void initmutex(void);
/*
* SIGINT handler
*/
void onSigInt(int sig);

/**
* Close open semaphores before calling exit()
*/
void terminateClient(void);

/**
* Close open semaphores and unlink them before calling exit()
*/
void terminateServer(void);

void enter(int sem_num);
void leave(int sem_num);

#endif
