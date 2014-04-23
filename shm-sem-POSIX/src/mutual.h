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
 
#define SIZE sizeof(Request)
 
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

void enter1(void);
void enter2(void);
void enter3(void);
void leave1(void);
void leave2(void);
void leave3(void);

#endif
