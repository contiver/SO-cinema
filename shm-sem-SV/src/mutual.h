#ifndef MUTUAL_H
#define MUTUAL_H

#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "../../common/ipc.h"
 
#define SIZE sizeof(Request)
#define SHARED_KEY ftok("/tmp/shm-SV", 1)
 
void fatal(char *s);
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
